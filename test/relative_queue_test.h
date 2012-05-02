/*
 * File: relative_queue_test.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-04-26
 *
 */
#ifndef RELATIVE_QUEUE_TEST_H
#define RELATIVE_QUEUE_TEST_H

#include "../src/relative_queue.h"


void testInsertFiveElementsOutOfOrder(void);

void compareQueueStoredRanks(relative_queue_t* queue, int* expected_ranks);


#endif /* end of include guard: RELATIVE_QUEUE_TEST_H */


