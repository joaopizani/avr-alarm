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
#include <avrutils/timers-atmega168p.h>
#include "alarm.h"


/** Initializes the queue used by the alarm.
 */
void alarm_init(void) {
    event_t e;
    e.rank = 0;
    e.handler = NULL;
    alarm_queue = relative_queue_create(e);

    alarm_timer_init();
}

void alarm_timer_init(void) {
    TCCR1A |= _BV(WGM11); // clear on compare match
    TIMSK1 |= _BV(OCIE1A); // enable compare match intr.

    // set compare match to 1ms
    const uint32_t alarmTimerPeriodCount = ALARM_TIMER_FREQ_HZ / 1000;
    const uint32_t alarmTimerToggleCount = alarmTimerPeriodCount / 2;
    OCR1A = alarmTimerToggleCount;

    // start timer
    TCCR1B = getTimerPrescaleBits(ALARM_PRESCALE);
}

/** Inserts a new element into the alarm's queue.
 */
void alarm_insert(time_ms_t timeout, handler_t handler) {
    event_t e;
    e.rank = timeout;
    e.handler = handler;
    relative_queue_insert(alarm_queue, e);
}

/** Alarm interrupt handler.
 */
inline void alarm_intr_handler(void) {
    event_t* head = relative_queue_head(alarm_queue);
    if(head == NULL) return;

    (head->rank)--;
    if(head->rank <= 0) {
        sei();
        handler_t f = head->handler;
        relative_queue_remove(alarm_queue, *head);
        (*f)();
    }
}

ISR(TIMER1_COMPA_vect) {
    alarm_intr_handler();
}


