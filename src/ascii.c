#include "libg13.h"
#include "libg13_private.h"

#include <stdlib.h>

Elem **ascii;

void ASCII(char c, char r[5]) {
    size_t i;
    char x = 0;
    char y = 0;
    for (i = 0; i < 5; ++i) {
        do { if (r[i]&1) _add_point(ascii[(size_t)c], (Point){x, y}); ++x;
        } while (r[i]>>=1);
        ++y; x = 0;
    }
}

Elem* get_ascii(char c) {
    if (!ascii[(size_t)c]->p && c != ' ')
        return ascii[0]; // Default
    return ascii[(size_t)c];
}

void _init_ascii() {
    size_t i;
    ascii = malloc(256 * sizeof(Elem*));
    for (i = 0; i < 256; ++i) {
        ascii[i] = (Elem*) malloc(sizeof(Elem));
        ascii[i]->p = NULL;
        ascii[i]->next = NULL;
        ascii[i]->count = 0;
        ascii[i]->x_off = 0;
        ascii[i]->y_off = 0;
    }

    ASCII(0  , (char[5]){14, 10, 10, 10, 14}); // Default

    ASCII(1  , (char[5]){10,  0,  0, 17, 14}); // :)
    ASCII(2  , (char[5]){10,  0,  0, 31,  0}); // :|
    ASCII(3  , (char[5]){10,  0,  0, 14, 17}); // :(

    ASCII(' ', (char[5]){ 0,  0,  0,  0,  0}); // 32
    ASCII('!', (char[5]){ 4,  4,  4,  0,  4}); // 33
    ASCII('"', (char[5]){10, 10,  0,  0,  0}); // 34
    ASCII('#', (char[5]){10, 31, 10, 31, 10}); // 35
    ASCII('$', (char[5]){30,  5, 14, 20, 15}); // 36
    ASCII('%', (char[5]){17,  8,  4,  2, 17}); // 37
    ASCII('&', (char[5]){ 2,  5, 22,  9, 22}); // 38
    ASCII(39 , (char[5]){ 4,  2,  0,  0,  0}); // 39
    ASCII('(', (char[5]){ 8,  4,  4,  4,  8}); // 40
    ASCII(')', (char[5]){ 2,  4,  4,  4,  2}); // 41
    ASCII('*', (char[5]){ 4, 31, 10, 17,  0}); // 42
    ASCII('+', (char[5]){ 4,  4, 31,  4,  4}); // 43
    ASCII(',', (char[5]){ 0,  0,  0,  4,  2}); // 44
    ASCII('-', (char[5]){ 0,  0, 14,  0,  0}); // 45
    ASCII('.', (char[5]){ 0,  0,  0,  0,  2}); // 46
    ASCII('/', (char[5]){16,  8,  4,  2,  1}); // 47
    ASCII('0', (char[5]){14, 17, 21, 17, 14}); // 48
    ASCII('1', (char[5]){ 6,  4,  4,  4,  4});
    ASCII('2', (char[5]){15, 16, 14,  1, 31});
    ASCII('3', (char[5]){14, 17, 12, 17, 14});
    ASCII('4', (char[5]){17, 17, 31, 16, 16});
    ASCII('5', (char[5]){31,  1, 15, 16, 15});
    ASCII('6', (char[5]){14,  1, 15, 17, 14});
    ASCII('7', (char[5]){31, 16, 16,  8,  4});
    ASCII('8', (char[5]){14, 17, 14, 17, 14});
    ASCII('9', (char[5]){14, 17, 30, 16, 16}); // 57
    ASCII(':', (char[5]){ 0,  4,  0,  4,  0}); // 58
    ASCII(';', (char[5]){ 0,  4,  0,  4,  2}); // 59
    ASCII('<', (char[5]){ 8,  4,  2,  4,  8}); // 60
    ASCII('=', (char[5]){ 0, 31,  0, 31,  0}); // 61
    ASCII('>', (char[5]){ 2,  4,  8,  4,  2}); // 62
    ASCII('?', (char[5]){14, 17, 12,  0,  4}); // 63
    ASCII('@', (char[5]){14, 21, 27,  5, 14}); // 64
    ASCII('A', (char[5]){14, 17, 31, 17, 17}); // 65
    ASCII('B', (char[5]){15, 17, 15, 17, 15});
    ASCII('C', (char[5]){14, 17,  1, 17, 14});
    ASCII('D', (char[5]){15, 17, 17, 17, 15});
    ASCII('E', (char[5]){31,  1,  7,  1, 31});
    ASCII('F', (char[5]){31,  1,  7,  1,  1});
    ASCII('G', (char[5]){14,  1, 25, 17, 14});
    ASCII('H', (char[5]){17, 17, 31, 17, 17});
    ASCII('I', (char[5]){31,  4,  4,  4, 31});
    ASCII('J', (char[5]){31,  8,  8,  9,  6});
    ASCII('K', (char[5]){17,  9,  7,  9, 17});
    ASCII('L', (char[5]){ 1,  1,  1,  1, 31});
    ASCII('M', (char[5]){17, 31, 21, 17, 17});
    ASCII('N', (char[5]){17, 19, 21, 25, 17});
    ASCII('O', (char[5]){14, 17, 17, 17, 14});
    ASCII('P', (char[5]){15, 17, 15,  1,  1});
    ASCII('Q', (char[5]){14, 17, 21, 14, 16});
    ASCII('R', (char[5]){15, 17, 15, 17, 17});
    ASCII('S', (char[5]){30,  1, 14, 16, 15});
    ASCII('T', (char[5]){31,  4,  4,  4,  4});
    ASCII('U', (char[5]){17, 17, 17, 17, 14});
    ASCII('V', (char[5]){17, 17, 17, 10,  4});
    ASCII('W', (char[5]){17, 17, 17, 21, 10});
    ASCII('X', (char[5]){17, 10,  4, 10, 17});
    ASCII('Y', (char[5]){17, 17, 10,  4,  4});
    ASCII('Z', (char[5]){31,  8,  4,  2, 31}); // 90
    ASCII('[', (char[5]){ 3,  1,  1,  1,  3}); // 91
    ASCII(92 , (char[5]){ 1,  2,  4,  8, 16}); // 47
    ASCII(']', (char[5]){24, 16, 16, 16, 24}); // 93
    ASCII('^', (char[5]){ 4, 10, 17,  0,  0}); // 94
    ASCII('_', (char[5]){ 0,  0,  0,  0, 31}); // 95
    ASCII('`', (char[5]){ 2,  4,  0,  0,  0}); // 96
    ASCII('{', (char[5]){24,  4,  6,  4, 24}); // 123
    ASCII('|', (char[5]){ 4,  4,  4,  4,  4}); // 124
    ASCII('}', (char[5]){ 3,  4, 12,  4,  3}); // 125
    ASCII('~', (char[5]){ 2, 21,  8,  0,  0}); // 126
}
