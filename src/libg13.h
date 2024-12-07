#ifndef GUARD_LIBG13_H
#define GUARD_LIBG13_H

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
typedef void(*g13_func_ptr_btn_all_t)(int, bool);

/*** g13.c ***/

int g13_init(void);
void g13_set_img(char*);
void g13_clear_lcd();
void g13_set_color(int, int, int);
void g13_render(void);
void g13_bind_key(int, g13_func_ptr_btn_t);
void g13_bind_stick(g13_func_ptr_stk_t);
void g13_bind_all_keys(g13_func_ptr_btn_all_t);
void g13_unbind_key(int);
void g13_unbind_stick();
void g13_unbind_all_keys();


/*** draw.c ***/

void g13_draw_char(unsigned int, unsigned int, char);
void g13_draw_sentence(unsigned int, unsigned int, const char*);
void g13_draw_circle(unsigned int, unsigned int, float);
void g13_draw_line(int, int, int, int);
void g13_draw_triangle(int, int, int, int, int, int);


#endif
