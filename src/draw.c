#include "libg13.h"
#include "libg13_private.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

Elem* elems = NULL;

Elem* _new_elem() {
    Elem* e = elems;
    if (!e) {
        elems = (Elem*) malloc(sizeof(Elem));
        e = elems;
    } else {
        while (e->next) e = e->next;
        e->next = (Elem*) malloc(sizeof(Elem));
        e = e->next;
    }
    e->p = NULL;
    e->next = NULL;
    e->count = 0;
    e->is_grapheme = 0;
    e->x_off = 0;
    e->y_off = 0;
    return e;
}

void _add_point(Elem* e, Point p) {
    (e->count)++;
    if (!e->p) e->p = malloc(sizeof(Point));
    else e->p = realloc(e->p, (e->count) * sizeof(Point));
    (e->p[e->count-1]).x = p.x;
    (e->p[e->count-1]).y = p.y;
}

void g13_draw_char(unsigned int x, unsigned int y, char c) {
    Elem* e = _new_elem();
    Elem* grapheme;
    grapheme = get_ascii(toupper(c));
    // The only thing that doesn't carry straight
    // across is 'next', but this should be NULL
    // in src and dst anyway since e is new.
    memcpy(e, grapheme, sizeof(Elem));
    e->x_off = x;
    e->y_off = y;
}

void g13_draw_sentence(unsigned int x, unsigned int y, char* c) {
    int charx = x;
    int i;
    for (i = 0; i < strlen(c); ++i) {
        if (c[i] == ' ') { charx += 3; continue; } // Smaller spaces
        g13_draw_char(charx, y, c[i]);
        charx += 6;
    }
}

void g13_draw_circle(unsigned int x, unsigned int y, float r) {
    Elem* e = _new_elem();
    float cx = 0., cy = 0.;
    for (cx = -r; cx < r; cx+=0.001f) {
        cy = sqrtf((powf(r, 2.f) - powf(cx, 2.f)));
        _add_point(e, (Point){(int)(cx+0.5f) + (float)x, (int)(cy+(float)y+0.5f)});
        _add_point(e, (Point){(int)(cx+0.5f) + (float)x, (int)(-cy-0.5f+(float)y)});
    }
}

void g13_draw_line(int x0, int y0, int x1, int y1) {
    Point p;
    Elem* e = _new_elem();
    unsigned int xt, yt;
    if (x0 > x1) {xt = x0; yt = y0; x0 = x1; y0 = y1; x1 = xt; y1 = yt;}
    float dydx = (float)(y1 - y0)/(float)(x1 - x0);
    int x = x0;
    float yf  = (float) y0;
    int yint = (int) yf;
    while (x <= x1) {
        p.x = x; p.y = (dydx > 0) ? (int) (yf + 0.5f) : (int) (yf - 0.5);
        _add_point(e, p);
        yf += dydx;
        if (dydx > 0 && yf > y1) break;
        if (dydx < 0 && yf < y1) break;
        while (yint + 1 < (int) (yf + 0.5f) - 1) {
            yint = (dydx > 0) ? p.y++ : p.y--;
            if (yint > y1) break;
            _add_point(e, p);
        }
        yint = (int) yf;
        ++x;
    }
}

void g13_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2) {
    g13_draw_line(x0, y0, x1, y1);
    g13_draw_line(x1, y1, x2, y2);
    g13_draw_line(x2, y2, x0, y0);
}

/* Render drawn shapes onto LCD */
void render(G13LCD* lcd) {
    Elem* e = elems;
    Point* p;

    while (e) {
        p = e->p;
        while (p < e->p + e->count) {
            (lcd->img)[e->x_off + p->x + G13LCD_W_PX*((e->y_off + p->y) / 8)] |= 1<<((e->y_off + p->y)%8);
            ++p;
        }
        e = e->next;
    }
}

void clear_elems() {
    for (Elem *e = elems; e != NULL; e = e->next)
        free(e);
    elems = NULL;
}

G13LCD *new_lcd(void) {
    return (G13LCD*) calloc(1, sizeof(G13LCD));
}
