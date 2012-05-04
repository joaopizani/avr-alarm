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

#define ALARM_PRESCALE 1024
#define ALARM_TIMER_FREQ_HZ (F_CPU / ALARM_PRESCALE)

relative_queue_t* alarm_queue;


typedef uint16_t time_ms_t;


/** Initializes the queue used by the alarm.
 */
void alarm_init(void);

void alarm_timer_init(void);


/** Inserts a new element into the alarm's queue.
 */
void alarm_insert(time_ms_t timeout, handler_t handler);

/** Alarm interrupt handler.
 */
inline void alarm_intr_handler(void);


#endif /* end of include guard: ALARM_H */