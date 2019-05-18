#ifndef GUARD_LIBG13_PRIVATE_H
#define GUARD_LIBG13_PRIVATE_H

#define G13LCD_SZ_BYTE 960


/*** g13.c ***/


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


/*** ascii.c ***/

void _init_ascii(void);

#endif
