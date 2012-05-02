/*
 * File: relative_queue_test.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-04-26
 *
 */
#include <stdio.h>
#include "relative_queue_test.h"
#include "../src/relative_queue.h"



void testInsertFiveElementsOutOfOrder(void) {
    event_t e;
    e.handler = NULL;

    e.rank = 10;
    relative_queue_t* q = relative_queue_create(e);

    e.rank = 12;
    relative_queue_insert(q, e);

    e.rank = 27;
    relative_queue_insert(q, e);

    e.rank = 24;
    relative_queue_insert(q, e);

    e.rank = 7;
    relative_queue_insert(q, e);

    int expectedRanks[] = {7, 3, 2, 12, 3};
    compareQueueStoredRanks(q, expectedRanks);
}


void compareQueueStoredRanks(relative_queue_t* queue, int* expected_ranks) {
    unsigned int cnt = 0;
    event_bin_t* it = queue->head;

    do {
        if(it->e.rank != *expected_ranks) {
            printf(""
                "Queue rank mismatch:\n"
                    "\tPosition = %d\n"
                    "\tExpected rank = %d\n"
                    "\tActual rank = %d\n",
                cnt, *expected_ranks, it->e.rank);
        }

        cnt++;
        it = it->next;
        expected_ranks++;
    } while (it != (event_bin_t*) 0);
}

