#ifndef GUARD_LIBG13_H
#define GUARD_LIBG13_H

#include <stdint.h>
#include <stdbool.h>


// LCD dimensions
extern const unsigned char G13_LCD_PX_X;
extern const unsigned char G13_LCD_PX_Y;

// Keys
enum G13Keys {
    G1, G2, G3, G4, G5, G6, G7, G8, G9, G10,
    G11, G12, G13, G14, G15, G16, G17, G18, G19, G20,
    G21, G22,
    M1, M2, M3, MR,
    T1, T2, T3, T4,
    CLICK1, CLICK2,
    ROUND, BRIGHT,
    N_KEYS /* Equal to number of elements */
};

// Input callback function types
typedef void(*g13_func_ptr_btn_t)(bool);
typedef void(*g13_func_ptr_stk_t)(unsigned char, unsigned char);
typedef void(*g13_func_ptr_btn_all_t)(int32_t, bool);

/*** g13.c ***/

int32_t g13_init(void);
void g13_set_img(char*);
void g13_clear_lcd();
void g13_set_color(int32_t, int32_t, int32_t);
void g13_set_brightness(int32_t);
void g13_render(void);
void g13_bind_key(int32_t, g13_func_ptr_btn_t);
void g13_bind_stick(g13_func_ptr_stk_t);
void g13_bind_all_keys(g13_func_ptr_btn_all_t);
void g13_unbind_key(int32_t);
void g13_unbind_stick();
void g13_unbind_all_keys();


/*** draw.c ***/

void g13_draw_char(uint32_t, uint32_t, char);
void g13_draw_char_scaled(uint32_t, uint32_t, char, uint32_t);
void g13_draw_string(uint32_t, uint32_t, const char*);
void g13_draw_string_scaled(uint32_t, uint32_t, const char*, uint32_t);
void g13_draw_circle(uint32_t, uint32_t, float);
void g13_draw_line(int32_t, int32_t, int32_t, int32_t);
void g13_draw_triangle(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);


#endif
