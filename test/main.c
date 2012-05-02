/*
 * File: main.c
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-02
 *
 */
#include <stdio.h>
#include "debug.h"
#include "relative_queue_test.h"


int main(void) {
    // necessary for testing
    stdout = stdin = fdevopen(debug_putc, debug_getc);

    testInsertFiveElementsOutOfOrder();

    printf("Tests finished. If no error messages were displayed, success!\n");

    return 0;
}
