#include "libg13.h"
#include "libg13_private.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <pthread.h>
#include <string.h>
#include "libusb-1.0/libusb.h"


const unsigned char G13_LCD_PX_X = 160;
const unsigned char G13_LCD_PX_Y = 43;

libusb_device **devs;
struct input_event _event;

G13LCD* lcd;

libusb_device_handle *handle;

g13_func_ptr_btn_t     *bound_keys;
g13_func_ptr_stk_t     *_stick;
g13_func_ptr_btn_all_t *_all_keys;

static char stick_x;
static char stick_y;

Elem** ascii;

/* key_state[0] - key_state[4] corresponds with buffer[3] - buffer[7] */
static char key_state[5];

void _init_lcd() {
    lcd = new_lcd();
    g13_clear_lcd();
    g13_render();
}

void set_lcd_img(libusb_device_handle *handle, char mask, char *file) {
    if (handle == NULL)
        return;

    uint32_t i = 0;
    if (file) {
        FILE* f = fopen(file, "r");
        while (i < 960)
            lcd->img[i++] = fgetc(f) | mask;
        fclose(f);
    } else {
        while (i < 960)
            lcd->img[i++] = 0;
    }
}

void g13_set_img(char *file) {
    set_lcd_img(handle, 0, file);
}

void g13_clear_lcd() {
    set_lcd_img(handle, 0, NULL);
    clear_elems();
}

void g13_render() {
    if (handle == NULL)
        return;

    unsigned char img2[960+32];
    int32_t written;
    size_t i;
    int32_t error;
    render(lcd);
    img2[0] = 0x03;
    i = 1; while (i < 32) img2[i++] = 0;  /* IMPORTANT! */
    memcpy(img2 + 32, lcd->img, 960);
    error = libusb_control_transfer(handle, 0, 9, 1, 0, 0, 0, 1000);
    error = libusb_interrupt_transfer(handle,
                                      LIBUSB_ENDPOINT_OUT | 2, img2,
                                      960 + 32,
                                      &written,
                                      1000);
    if (error) {
        fprintf(stderr, "Error communicating with G13\n");
    }
}

int32_t create_uinput() {
    struct uinput_user_dev uinp;
    int32_t ufile;
    uint32_t i;
    int32_t retcode;
    char name[] = "G13";

    ufile = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (ufile < 0) {
        fprintf(stderr, "Reading /dev/uinput failed\n");
        exit(1);
    }

    memset(&uinp, 0, sizeof(uinp));
    strncpy(uinp.name, name, UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 1;
    uinp.id.bustype = BUS_USB;
    uinp.id.product = 0x1;
    uinp.id.vendor = 0x1;

    ioctl(ufile, UI_SET_EVBIT, EV_KEY);
    ioctl(ufile, UI_SET_EVBIT, EV_REL);
    ioctl(ufile, UI_SET_RELBIT, REL_X);
    ioctl(ufile, UI_SET_RELBIT, REL_Y);
    ioctl(ufile, UI_SET_MSCBIT, MSC_SCAN);
    ioctl(ufile, UI_SET_ABSBIT, ABS_X);
    ioctl(ufile, UI_SET_ABSBIT, ABS_Y);

    for (i = 0; i < 256; i++)
        ioctl(ufile, UI_SET_KEYBIT, i);

    ioctl(ufile, UI_SET_KEYBIT, BTN_THUMB);

    retcode = write(ufile, &uinp, sizeof(uinp));

    if (retcode < 0)
        return -1;

    retcode = ioctl(ufile, UI_DEV_CREATE);

    if (retcode)
        return -1;

    return ufile;
}

void send_event(int32_t infile, int32_t type, int32_t code, int32_t val) {
    memset(&_event, 0, sizeof(_event));
    gettimeofday(&_event.time, 0 );
    _event.type = type;
    _event.code = code;
    _event.value = val;
    write(infile, &_event, sizeof(_event));
}

void g13_set_color(int32_t red, int32_t green, int32_t blue) {
    if (handle == NULL)
        return;

    int32_t error;
    unsigned char usb_data[] = { 5, 0, 0, 0, 0 };
    usb_data[1] = red;
    usb_data[2] = green;
    usb_data[3] = blue;

    error = libusb_control_transfer(handle,
            LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x307, 0,
            usb_data, 5, 1000);
    if (error != 5) {
        exit(1);
        return;
    }
}

void g13_bind_key(int32_t k, g13_func_ptr_btn_t f) {
    bound_keys[k] = f;
}

void g13_bind_stick(g13_func_ptr_stk_t f) {
    *_stick = f;
}

void g13_bind_all_keys(g13_func_ptr_btn_all_t f) {
    *_all_keys = f;
}

void g13_unbind_key(int32_t k) {
    bound_keys[k] = NULL;
}

void g13_unbind_stick() {
    *_stick = NULL;
}

void g13_unbind_all_keys() {
    *_all_keys = NULL;
}

#define UPDATE_KEY(key, bufidx, mask) \
    if ((buffer[(bufidx)] & (mask)) != (key_state[((bufidx) - 3)] & (mask))) { \
        key_state[((bufidx) - 3)] ^= (mask); \
        if (bound_keys[(key)]) { \
            bound_keys[(key)]((buffer[(bufidx)] & (mask))); \
        } \
        if (*_all_keys) { \
            (*_all_keys)((key), (buffer[(bufidx)] & (mask))); \
        } \
    }

int32_t read_keys(libusb_device_handle *handle) {
    if (handle == NULL)
        return 0;

    unsigned char buffer[8];
    int32_t size;
    uint32_t i, j;
    int32_t error;

    memset(buffer, 0, sizeof(buffer));

    error = libusb_interrupt_transfer( handle,
            LIBUSB_ENDPOINT_IN | 1, buffer, 8,
            &size, 100);

    if (error == LIBUSB_ERROR_TIMEOUT) {
        return 0;
    } else if (error) {
        exit(1);
    }

    // G1..G8   in byte 3, G1  = least sig bit
    // G9..G16  in byte 4, G9  = least sig bit
    // G17..G22 in byte 5, G17 = least sig bit
    // two most sig bits of B5: less sig not used? most = any input.

    /*
    printf("\e[H\e[J");
    for (uint32_t k = 0; k < 8; ++k) {
        printf("%d: ", k);
        for (i = 0; i < 8; ++i) {
            if ((buffer[k] >> i) & 0x1) {
                printf("1");
            } else {
                printf("0");
            }
        }
        printf("\n");
    }
    */

    char is_pressed;

    /* G keys */
    /* Iterate over the buffers... */
    for (j = 0; j < 3; ++j) {
        /* Iterate over the bits... */
        for (i = 0; i < 8; ++i) {
            if (j == 2 && i == 7) {
                /*
                 * The most significant bit in buffer[5] does not
                 * map to a key, and is set on any input change.
                 */
                continue;
            }
            is_pressed = buffer[j + 3] & (1 << i);
            if (is_pressed != (key_state[j] & (1 << i))) {
                key_state[j] ^= (1 << i);
                if (bound_keys[G1 + j*8 + i]) {
                    bound_keys[G1 + j*8 + i](is_pressed);
                }
                if (*_all_keys) {
                    (*_all_keys)(G1 + j*8 + i, is_pressed);
                }
            }
        }
    }

    /*
     * UPDATE_KEY, e.g:
     *
     *  is_pressed = buffer[6] & 0x20;
     *  if (is_pressed != key_state[3] & 0x20) {
     *      key_state[3] ^= 0x20;
     *      if (bound_keys[M1]) {
     *          bound_keys[M1](is_pressed);
     *      }
     *  }
     */

    /* M keys */
    UPDATE_KEY(M1, 6, 0x20);
    UPDATE_KEY(M2, 6, 0x40);
    UPDATE_KEY(M3, 6, 0x80);
    UPDATE_KEY(MR, 7, 0x01);

    /* Round button */
    UPDATE_KEY(ROUND, 6, 0x01);

    /* Top buttons */
    UPDATE_KEY(T1, 6, 0x02);
    UPDATE_KEY(T2, 6, 0x04);
    UPDATE_KEY(T3, 6, 0x08);
    UPDATE_KEY(T4, 6, 0x10);

    /* Brightness */
    UPDATE_KEY(BRIGHT, 7, 0x20 | 0x40);

    /* Mouse buttons */
    UPDATE_KEY(CLICK1, 7, 0x02);
    UPDATE_KEY(CLICK2, 7, 0x04);

    /* Bytes 1 and 2 are X and Y resp */
    if ((buffer[1] || buffer[2]) && *_stick && (buffer[1] != stick_x || buffer[2] != stick_y)) {
        stick_x = buffer[1];
        stick_y = buffer[2];
        (*_stick)(buffer[1], buffer[2]);
    }

    return 0;
}

void _read_keys(void) {
    if (handle == NULL)
        return;

    while (1) {
        read_keys(handle);
        /* Sleep for 1/1000 seconds */
        usleep(1000);
    }
}

int32_t g13_init(void) {
    int32_t ndev;
    uint32_t i;
    struct libusb_device_descriptor desc;
    pthread_t keys_thread;

    handle = NULL;

    if (libusb_init(NULL)) {
        fprintf(stderr, "Failed to initialise libusb\n");
        return 1;
    }

    bound_keys = calloc(N_KEYS, sizeof(g13_func_ptr_btn_t));
    _stick = calloc(1, sizeof(g13_func_ptr_stk_t));
    _all_keys = calloc(1, sizeof(g13_func_ptr_btn_all_t));

    /*create_uinput();*/

    ndev = libusb_get_device_list(NULL, &devs);
    for (i = 0; i < ndev; i++) {
        int32_t rx = libusb_get_device_descriptor(devs[i], &desc);
        if (desc.idVendor == 0x046d && desc.idProduct == 0xc21c) {
            rx = libusb_open(devs[i], &handle);

            if (rx != 0) {
                fprintf(stderr, "Could not create USB handle (requires root)\n");
                return 2;
            }

            if (libusb_kernel_driver_active(handle, 0) == 1)
                if (libusb_detach_kernel_driver(handle, 0) == 0)
                    fprintf(stderr, "Kernel driver detached");

            rx = libusb_claim_interface(handle, 0);
        }
    }

    libusb_free_device_list(devs, 1);

    if (handle == NULL) {
        return 3;
    }

    _init_ascii();
    _init_lcd();

    if (pthread_create(&keys_thread, NULL, (void *(*)(void*))&_read_keys, NULL)) {
        fprintf(stderr, "Could not create input thread\n");
        return 4;
    }

    return 0;
}

