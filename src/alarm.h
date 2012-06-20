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
#define ALARM_TIMER_FREQ_KHZ (ALARM_TIMER_FREQ_HZ / 1000)

//TODO magical number, we shall define it based on the memory of the device and the
//size of the event struct
#define ALARM_MAX_EVENTS 20


relative_queue_t* alarm_queue;


typedef uint16_t time_ms_t;


/** Initializes the queue used by the alarm, with user-defined period between 1ms and 1000ms
 */
void alarm_init(time_ms_t period);

/** Starts the underlying alarm timer. At this moment the alarm starts counting time.
 */
void alarm_timer_start(void);

/** Inserts a new element into the alarm's queue.
 * Receives as argument the timeout in miliseconds, a pointer to a handler function and
 * a pointer to an argument which will be passed to the handler function.
 */
void alarm_insert(time_ms_t timeout, handler_t handler, void* arg_ptr);

/** Returns the amount of time that STILL has to pass until the last deadline is reached.
*/
time_ms_t alarm_until_last_deadline(void);

/** Alarm interrupt handler.
 */
inline void alarm_intr_handler(void);


#endif /* end of include guard: ALARM_H */
