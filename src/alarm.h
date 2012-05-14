/*
 * File: alarm.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-04
 *
 */
#ifndef ALARM_H
#define ALARM_H

#include <stdlib.h>
#include "relative_queue.h"

#define ALARM_PRESCALE 256
#define ALARM_TIMER_FREQ_HZ (F_CPU / ALARM_PRESCALE)

relative_queue_t* alarm_queue;


typedef uint16_t time_ms_t;


/** Initializes the queue used by the alarm.
 */
void alarm_init(void);

/** Starts the underlying alarm timer. At this moment the alarm starts counting time.
 */
void alarm_timer_start(void);

/** Inserts a new element into the alarm's queue.
 * Receives as argument the timeout in miliseconds, a pointer to a handler function and
 * a pointer to an argument which will be passed to the handler function.
 */
void alarm_insert(time_ms_t timeout, handler_t handler, void* arg_ptr);

/** Alarm interrupt handler.
 */
inline void alarm_intr_handler(void);


#endif /* end of include guard: ALARM_H */
