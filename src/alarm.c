/*
 * File: alarm.h
 *
 * Author: Tarcisio Fischer
 * Date: 2012-05-04
 *
 */
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr-utils/timers-atmega168.h>
#include "relative_queue.h"
#include "alarm.h"


/** Initializes the queue used by the alarm.
 */
void alarm_init(time_ms_t period) {
    event_t e;
    e.rank = 0;
    e.handler = NULL;
    alarm_queue = relative_queue_create(e);

    // configuring the underlying alarm timer
    TCCR1A = 0x00; // just to guarantee
    TCCR1B |= _BV(WGM12); // clear on compare match
    TIMSK1 |= _BV(OCIE1A); // enable compare match intr.

    // set compare match to 1ms
    const uint16_t alarmTimerPeriodCount = period * ALARM_TIMER_FREQ_KHZ;
    OCR1A = alarmTimerPeriodCount;
}

void alarm_timer_start(void) {
    // start timer
    TCCR1B |= getTimer1PrescaleBits_atmega168(ALARM_PRESCALE);
}

void alarm_insert(time_ms_t timeout, handler_t handler, void* arg_ptr) {
    event_t e;
    e.rank = timeout;
    e.handler = handler;
    e.arg_ptr = arg_ptr;

    // busy-wait for a slot in the queue. The size variable and the function call
    // need all to be VOLATILE AND LITERAL, with no optimizations whatsoever.
    // the cast to volatile is needed so that GCC don't read this from a register, and
    // perform a MEMORY read at every while iteration. This way, a write in the interrupt
    // context can be noticed here.
    while(   *( (volatile uint8_t *) &(alarm_queue->size) )  >=  ALARM_MAX_EVENTS   );

    relative_queue_insert(alarm_queue, e);
}

time_ms_t alarm_until_last_deadline(void) {
    return relative_queue_rank_sum(alarm_queue);
}

inline void alarm_intr_handler(void) {
    event_t* head = relative_queue_head(alarm_queue);
    if(head == NULL) return;

    if((head->rank) == 0) {
        handler_t f = head->handler;
        void* arg_ptr = head->arg_ptr;
        relative_queue_remove(alarm_queue, *head);

        sei();

        if(f != NULL) {
            (*f)(arg_ptr);
        }
    } else {
        (head->rank)--;
    }
}

/// main alarm timer interrupt handler.
ISR(TIMER1_COMPA_vect) {
    alarm_intr_handler();
}

