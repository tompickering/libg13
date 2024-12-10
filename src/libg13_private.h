#ifndef GUARD_LIBG13_PRIVATE_H
#define GUARD_LIBG13_PRIVATE_H

#define G13LCD_SZ_BYTE 960

#include <stdint.h>


/*** g13.c ***/


/*** draw.c ***/

typedef struct G13LCD G13LCD;
typedef struct Elem Elem;
typedef struct Point Point;

struct G13LCD {
    char img[G13LCD_SZ_BYTE];
};

struct Elem {
    Point* p;
    uint32_t count;
    Elem* next;
    uint32_t x_off;
    uint32_t y_off;
};

struct Point {
    char x;
    char y;
};

extern Elem **ascii;
extern Elem **icons;

Elem* get_ascii(char);
void _add_point(Elem*, Point);
void _scale_elem(Elem*, Elem*, uint32_t);
void render(G13LCD*);
void clear_elems();
G13LCD *new_lcd(void);


/*** ascii.c ***/

void _init_ascii(void);

/*** icon.c ***/

void _init_icons(void);
Elem* _get_icon(int32_t);

#endif
