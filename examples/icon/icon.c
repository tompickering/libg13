#include <stdio.h>
#include <unistd.h>

#include <libg13.h>

char* word = "Hello, world!";

int main(int argc, char** argv) {
    g13_init();
    int id0 = g13_define_icon((uint32_t[]){1, 2, 3, 4, 5, 6, 7, 8}, 8);
    int id1 = g13_define_icon((uint32_t[]){1, 2, 4, 8, 16, 32, 64, 128}, 8);

    int id2 = g13_define_icon((uint32_t[]){1025, 514, 248, 508, 428, 508, 248, 136, 112, 514, 1025}, 11);

    g13_draw_icon(id0, 10, 10);
    g13_draw_icon(id1, 30, 10);
    g13_draw_icon(id2, 90, 10);
    g13_render();

    return 0;
}
