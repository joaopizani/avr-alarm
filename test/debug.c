/*
 * File: debug.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-04-26
 *
 */
#include <stdio.h>
#include "debug.h"


int debug_putc(char c, FILE* stream) {
    DEBUG_OUTPUT = c;
    return 0;
}

int debug_getc(FILE* stream) {
    int c;
    c = (int) DEBUG_OUTPUT;

    return c;
}

