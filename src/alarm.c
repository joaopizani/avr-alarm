/*
 * File: alarm.h
 *
 * Author: Tarcisio Fischer
 * Date: 2012-05-04
 *
 */
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <timers-atmega168p.h>
#include "alarm.h"


/** Initializes the queue used by the alarm.
 */
void alarm_init(void) {
    event_t e;
    e.rank = 0;
    e.handler = NULL;
    alarm_queue = relative_queue_create(e);

    // configuring the underlying alarm timer
    TCCR1B |= _BV(WGM12); // clear on compare match
    TIMSK1 |= _BV(OCIE1A); // enable compare match intr.

    // set compare match to 1ms
    const uint16_t alarmTimerPeriodCount = ALARM_TIMER_FREQ_HZ / 1000;
    OCR1A = alarmTimerPeriodCount;
}

void alarm_timer_start(void) {
    // start timer 1024
    TCCR1B |= 0b00000101; // TODO change to function specific of TIMER1
}

void alarm_insert(time_ms_t timeout, handler_t handler) {
    event_t e;
    e.rank = timeout;
    e.handler = handler;
    relative_queue_insert(alarm_queue, e);
}

inline void alarm_intr_handler(void) {
    event_t* head = relative_queue_head(alarm_queue);
    if(head == NULL) return;

    if((head->rank) == 0) {
        handler_t f = head->handler;
        relative_queue_remove(alarm_queue, *head);
        sei();
        (*f)();
    }

    (head->rank)--;
}

/// main alarm timer interrupt handler.
ISR(TIMER1_COMPA_vect) {
    alarm_intr_handler();
}

