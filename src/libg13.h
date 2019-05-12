#ifndef GUARD_LIBG13_H
#define GUARD_LIBG13_H

#define G13LCD_SZ_BYTE 960
#define G13LCD_W_PX 160
#define G13LCD_H_PX  43


// Input callback function types
typedef void(*func_ptr_t)(void);
typedef void(*func_stick_ptr_t)(unsigned char, unsigned char);


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



/*** g13.c ***/

int g13_init(void);
void g13_set_img(char*);
void g13_clear_lcd();
void g13_set_colour(int, int, int);
void g13_render(void);
void g13_bind_key(int, func_ptr_t);
void g13_bind_stick(func_stick_ptr_t);



/*** draw.c ***/

typedef struct G13LCD G13LCD;
typedef struct Elem Elem;
typedef struct Point Point;

struct G13LCD {
    unsigned char img[G13LCD_SZ_BYTE];
};

struct Elem {
    Point* p;
    unsigned int count;
    Elem* next;
    char is_grapheme;
    unsigned int x_off;
    unsigned int y_off;
};

struct Point {
    unsigned char x;
    unsigned char y;
};

Elem** ascii;

Elem* get_ascii(char);
void _add_point(Elem*, Point);
void render(G13LCD*);
void clear_elems();
G13LCD *new_lcd(void);

void g13_draw_char(unsigned int, unsigned int, char);
void g13_draw_sentence(unsigned int, unsigned int, char*);
void g13_draw_circle(unsigned int, unsigned int, float);
void g13_draw_line(int, int, int, int);
void g13_draw_triangle(int, int, int, int, int, int);



/*** ascii.c ***/

void _init_ascii(void);

#endif
