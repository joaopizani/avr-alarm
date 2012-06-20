/*
 * relative_queue.h
 *
 * João Paulo Pizani Flor <joaopizani@gmail.com>
 * 2011-04-22
 */
#ifndef RELATIVE_QUEUE_H
#define RELATIVE_QUEUE_H

#include <stdint.h>
#include <stdio.h>


typedef void (*handler_t)(void*);

typedef struct _event_t {
    uint16_t rank;
    void* arg_ptr;
    handler_t handler;
} event_t;

typedef struct _event_bin_t {
    event_t e;
    struct _event_bin_t* next;
} event_bin_t;

typedef struct _relative_queue_t {
    event_bin_t* head;
    event_bin_t* last;
    uint8_t size;
} relative_queue_t;


/** Creates a singleton queue, with a COPY of the specified element
 * We cannot create empty queues.
 */
relative_queue_t* relative_queue_create(event_t e);

/** Inserts a COPY of the specified element into the queue.
 * The queue pointed to by q must not be empty. That's guaranteed
 * if it was created with relative_queue_create(event_t e);.
 */
void relative_queue_insert(relative_queue_t* q, event_t e);

/** Removes the first element in the queue that matches with the given event.
 * The queue pointed to by q must not be empty.
 */
void relative_queue_remove(relative_queue_t* q, event_t e);

/** Gets the head (first element) of a queue.
 * Returns a pointer to the first element, allowing it to be modified.
 */
event_t* relative_queue_head(relative_queue_t* q);

/** Obtains the last element of a queue.
 * Returns a pointer, alowing it to be modified.
 */
event_t* relative_queue_last(relative_queue_t* q);

/** Gets the absoulte (non-relative) rank of the last element
 * in the queue.
 */
uint16_t relative_queue_rank_sum(relative_queue_t* q);

/** Returns the size (number of elements) of a queue.
 */
uint8_t relative_queue_size(relative_queue_t* q);


// Prints a relative queue to a stream device
void print_queue(FILE* out, relative_queue_t* q);


#endif /* end of include guard: RELATIVE_QUEUE_H */
