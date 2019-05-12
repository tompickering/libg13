#include <stdio.h>
#include <unistd.h>

#include <libg13.h>

char* word = "Hello, world!";

int main(int argc, char** argv) {
    if (argc > 1)
        word = argv[1];
    g13_init();
    g13_draw_circle(100, 20, 10);
    g13_draw_triangle(10, 5, 40, 10, 15, 20);
    g13_draw_line(60, 10, 80, 20);
    g13_draw_sentence(5, 30, word);
    g13_render();
    return 0;
}
