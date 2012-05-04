/*
 * File: alarm_test.c
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-04
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "assert_helpers.h"
#include "alarm_test.h"
#include "../src/alarm.h"
#include "../src/relative_queue.h"


void testAlarmInitButNotStartHasZeroedHead(void) {
    alarm_init();

    if(!assert_not_null(alarm_queue)) return;

    event_t* head = relative_queue_head(alarm_queue);

    if(!assert_equal_uint16(0, head->rank)) return;
    if(!assert_is_null(head->handler)) return;
}

void testAlarmWithoutInsertingAnythingGetNextOfHead(void) {
    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* after_head = head->next;
    if(!assert_is_null(after_head)) return;
}

void testFollowerOfHeadAfterInsertingOne(void) {
    handler_t cafe_handler = (handler_t) 0xCAFE;
    uint16_t cafe_timeout = 300;
    alarm_insert(cafe_timeout, cafe_handler);

    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* follow_head = head->next;
    if(!assert_not_null(follow_head)) return;

    if(!assert_equal_uint16(cafe_timeout, follow_head->e.rank)) return;

    if(!assert_equal_ptr(cafe_handler, follow_head->e.handler)) return;
}

void testAlarmInsertionOfThreeEvents(void) {
    /// one has already been inserted in the previous test
    uint16_t e1_timeout = 300; // has been inserted in the previuos test
    uint16_t e2_timeout = 250;
    uint16_t e3_timeout = 500;

    handler_t e1_handler = (handler_t) 0xCAFE; // previuos test case
    handler_t e2_handler = (handler_t) 0xB0B0;
    handler_t e3_handler = (handler_t) 0xDEAD;

    printf("Before inserting events in que alarm_queue\n"); //TODO dbg

    alarm_insert(e2_timeout, e2_handler);
    alarm_insert(e3_timeout, e3_handler);


    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* bin1 = head->next;
    if(!assert_not_null(bin1)) return;
    if(!assert_equal_uint16(e2_timeout, bin1->e.rank)) return;
    if(!assert_equal_ptr(e2_handler, bin1->e.handler)) return;

    event_bin_t* bin2 = bin1->next;
    if(!assert_not_null(bin2)) return;
    if(!assert_equal_uint16(50, bin2->e.rank)) return;
    if(!assert_equal_ptr(e1_handler, bin2->e.handler)) return;

    event_bin_t* bin3 = bin2->next;
    if(!assert_not_null(bin3)) return;
    if(!assert_equal_uint16(200, bin3->e.rank)) return;
    if(!assert_equal_ptr(e3_handler, bin3->e.handler)) return;
}

