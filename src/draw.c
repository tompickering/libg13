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

void _scale_elem(Elem* dst, Elem* src, uint32_t scale) {
    dst->count = 0;
    dst->is_grapheme = src->is_grapheme;
    dst->x_off = src->x_off;
    dst->y_off = src->y_off;

    for (uint32_t i_src = 0; i_src < src->count; ++i_src) {
        const Point *p = &(src->p[i_src]);
        for (uint32_t j = 0; j < scale; ++j) {
            for (uint32_t i = 0; i < scale; ++i) {
                _add_point(dst, (Point){p->x*scale+i, p->y*scale+j});
            }
        }
    }
}

void g13_draw_char(uint32_t x, uint32_t y, char c) {
    g13_draw_char_scaled(x, y, c, 1);
}

void g13_draw_char_scaled(uint32_t x, uint32_t y, char c, uint32_t scale) {
    Elem* e = _new_elem();
    Elem* grapheme;
    grapheme = get_ascii(toupper(c));
    if (scale == 1) {
        // The only thing that doesn't carry straight
        // across is 'next', but this should be NULL
        // in src and dst anyway since e is new.
        memcpy(e, grapheme, sizeof(Elem));
    } else {
        _scale_elem(e, grapheme, scale);
    }
    e->x_off = x;
    e->y_off = y;
}

void g13_draw_string(uint32_t x, uint32_t y, const char* c) {
    g13_draw_string_scaled(x, y, c, 1);
}

void g13_draw_string_scaled(uint32_t x, uint32_t y, const char* c, uint32_t scale) {
    uint32_t charx = x;
    size_t i;
    for (i = 0; i < strlen(c); ++i) {
        if (c[i] == ' ') { charx += 3; continue; } // Smaller spaces
        g13_draw_char_scaled(charx, y, c[i], scale);
        charx += 6*scale;
    }
}

void g13_draw_circle(uint32_t x, uint32_t y, float r) {
    Elem* e = _new_elem();
    float cx = 0., cy = 0.;
    for (cx = -r; cx < r; cx+=0.001f) {
        cy = sqrtf((powf(r, 2.f) - powf(cx, 2.f)));
        _add_point(e, (Point){(char)(cx+0.5f) + (float)x, (char)(cy+(float)y+0.5f)});
        _add_point(e, (Point){(char)(cx+0.5f) + (float)x, (char)(-cy-0.5f+(float)y)});
    }
}

void g13_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
    Point p;
    Elem* e = _new_elem();
    int32_t xt, yt;
    if (x0 > x1) {xt = x0; yt = y0; x0 = x1; y0 = y1; x1 = xt; y1 = yt;}
    float dydx = (float)(y1 - y0)/(float)(x1 - x0);
    int32_t x = x0;
    float yf  = (float) y0;
    int32_t yint = (int32_t) yf;
    while (x <= x1) {
        p.x = x; p.y = (dydx > 0) ? (int32_t) (yf + 0.5f) : (int32_t) (yf - 0.5);
        _add_point(e, p);
        yf += dydx;
        if (dydx > 0 && yf > y1) break;
        if (dydx < 0 && yf < y1) break;
        while (yint + 1 < (int32_t) (yf + 0.5f) - 1) {
            yint = (dydx > 0) ? p.y++ : p.y--;
            if (yint > y1) break;
            _add_point(e, p);
        }
        yint = (int32_t) yf;
        ++x;
    }
}

void g13_draw_triangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    g13_draw_line(x0, y0, x1, y1);
    g13_draw_line(x1, y1, x2, y2);
    g13_draw_line(x2, y2, x0, y0);
}

void g13_draw_icon(int32_t id, uint32_t x, uint32_t y) {
    Elem* icon = _get_icon(id);

    if (icon == NULL) {
        return;
    }

    Elem* e = _new_elem();

    memcpy(e, icon, sizeof(Elem));

    e->x_off = x;
    e->y_off = y;
}

/* Render drawn shapes onto LCD */
void render(G13LCD* lcd) {
    Elem* e = elems;
    Point* p;

    while (e) {
        p = e->p;
        while (p < e->p + e->count) {
            (lcd->img)[e->x_off + p->x + G13_LCD_PX_X*((e->y_off + p->y) / 8)] |= 1<<((e->y_off + p->y)%8);
            ++p;
        }
        e = e->next;
    }
}

void clear_elems() {
    Elem *next;
    for (Elem *e = elems; e != NULL; e = next) {
        next = e->next;
        free(e);
    }
    elems = NULL;
}

G13LCD *new_lcd(void) {
    return (G13LCD*) calloc(1, sizeof(G13LCD));
}
