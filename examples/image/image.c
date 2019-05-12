#include <stdio.h>
#include <unistd.h>

#include <libg13.h>

int main(int argc, char** argv) {
    printf("Run this from inside the image directory so we can find the image file\n");
    printf("(Otherwise this will likely segfault...)\n");
    g13_init();
    g13_set_colour(255, 0, 0);
    g13_clear_lcd();
    g13_set_img("fail.lpbm");
    g13_render();
    return 0;
}
