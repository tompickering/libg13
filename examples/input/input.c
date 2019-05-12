#include <stdio.h>
#include <unistd.h>

#include <libg13.h>

unsigned char x, y;

void red() {
    g13_set_colour(255, 0, 0);
}

void green() {
    g13_set_colour(0, 255, 0);
}

void blue() {
    g13_set_colour(0, 0, 255);
}

void stick(unsigned char new_x, unsigned char new_y) {
    printf("Stick position: %d %d\n", new_x, new_y);

    if (new_x != x || new_y != y) {
        x = new_x;
        y = new_y;
        g13_set_colour(x, 255, 255 - x);
        g13_clear_lcd();
        g13_draw_circle(2 + (int)(155.f * (float)x / 255.f),
                    2 + (int)(36.f * (float)y / 255.f), 2.);
        g13_render();
    }
}

void clear() {
    g13_clear_lcd();
    g13_render();
}

int main(int argc, char** argv) {
    g13_init();
    g13_bind_key(G1, red);
    g13_bind_key(G2, green);
    g13_bind_key(G3, blue);
    g13_bind_key(ROUND, clear);
    g13_bind_stick(stick);
    g13_clear_lcd();

    printf("Try moving the stick, or pressing buttons G1-3\n");

    while (1)
        usleep(1000000);

    return 0;
}
