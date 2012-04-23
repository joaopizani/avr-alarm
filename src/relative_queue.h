/*
 * relative_queue.h
 *
 * João Paulo Pizani Flor <joaopizani@gmail.com>
 * 2011-04-22
 */
#ifndef RELATIVE_QUEUE_H
#define RELATIVE_QUEUE_H

#include <stdint.h>


typedef void (*handler_t)(void);

typedef struct _event_t {
    uint16_t rank;
    handler_t handler;
} event_t;

typedef struct _event_bin_t {
    event_t e;
    struct _event_bin_t* next;
} event_bin_t;

typedef event_bin_t relative_queue_t;


/*
 * Creates a singleton queue, with a COPY of the specified element
 * We cannot create empty queues.
 */
event_bin_t* relative_queue_create(event_t e);

/*
 * Inserts a COPY of the specified element into the queue. The
 * queue pointed to by head must not be empty. That's guaranteed if
 * it was created with relative_queue_create.
 */
void relative_queue_insert(event_bin_t* head, event_t e);


#endif /* end of include guard: RELATIVE_QUEUE_H */
