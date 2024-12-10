#include "libg13.h"
#include "libg13_private.h"

#include <stdio.h>
#include <stdlib.h>

static const size_t MAX_ICONS = 255;

Elem **icons;

static size_t icons_head = 0;

void _init_icons() {
    icons = malloc(MAX_ICONS * sizeof(Elem*));
    icons_head = 0;
}

int32_t g13_define_icon(uint32_t* descriptor, uint8_t n) {
    if (icons_head >= MAX_ICONS) {
        fprintf(stderr, "Maximum number of icons defined\n");
        return -1;
    }

    int32_t id = icons_head++;

    icons[id] = malloc(sizeof(Elem));
    icons[id]->p = NULL;
    icons[id]->next = NULL;
    icons[id]->count = 0;
    icons[id]->x_off = 0;
    icons[id]->y_off = 0;

    char x = 0;
    char y = 0;
    for (size_t i = 0; i < n; ++i) {
        do { if (descriptor[i]&1) _add_point(icons[id], (Point){x, y}); ++x;
        } while (descriptor[i]>>=1);
        ++y; x = 0;
    }

    return id;
}

Elem* _get_icon(int32_t id) {
    if (id < 0 || id >= icons_head) {
        return NULL;
    }

    return icons[id];
}
