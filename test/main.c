/*
 * File: main.c
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-02
 *
 */
#include <stdio.h>
#include <avr/interrupt.h>
#include "debug.h"
#include "relative_queue_test.h"
#include "alarm_test.h"


int main(void) {
    /// necessary for testing
    stdout = stdin = fdevopen(debug_putc, debug_getc);

    /// relative_queue tests
    testInsertFiveElementsOutOfOrder();

    // we have to globally enable interrupts
    sei();

    /// alarm tests
    testAlarmInitButNotStartHasZeroedHead();
    testAlarmWithoutInsertingAnythingGetNextOfHead();
    testFollowerOfHeadAfterInsertingOne();
    testAlarmInsertionOfThreeEvents();
    testDummyHeadIsGoneAfterSomeTimeRunning();
    testDummyHandlerAndE2HandlerHaveRunAfterEnoughTime();

    printf("Tests finished. If no error messages were displayed, success!\n");

    return 0;
}
