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
#include <util/delay.h>
#include "assert_helpers.h"
#include "alarm_test.h"
#include "../src/alarm.h"
#include "../src/relative_queue.h"


#define TEST_E2_TIMOUT 50
#define TEST_E1_TIMOUT 80
#define TEST_E3_TIMOUT 97
#define TEST_E1_HANDLE_FUNC e1_handler
#define TEST_E2_HANDLE_FUNC e2_handler
#define TEST_E3_HANDLE_FUNC e3_handler

uint16_t test_e2_timeout = TEST_E2_TIMOUT;
uint16_t test_e1_timeout = TEST_E1_TIMOUT;
uint16_t test_e3_timeout = TEST_E3_TIMOUT;
handler_t test_e1_handler = TEST_E1_HANDLE_FUNC;
handler_t test_e2_handler = TEST_E2_HANDLE_FUNC;
handler_t test_e3_handler = TEST_E3_HANDLE_FUNC;

int handler_flag_value = 0;


void testAlarmInitButNotStartHasZeroedHead(void) {
    alarm_init();

    // guarantee that there's no zero at the handler, so that we don't reset.
    alarm_queue->head->e.handler = dummy_handler;

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
    alarm_insert(test_e1_timeout, test_e1_handler);

    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* follow_head = head->next;
    if(!assert_not_null(follow_head)) return;

    if(!assert_equal_uint16(test_e1_timeout, follow_head->e.rank)) return;

    if(!assert_equal_ptr(test_e1_handler, follow_head->e.handler)) return;
}

void testAlarmInsertionOfThreeEvents(void) {
    alarm_insert(test_e2_timeout, test_e2_handler);
    alarm_insert(test_e3_timeout, test_e3_handler);

    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    uint16_t acc_value = 0;

    event_bin_t* bin1 = head->next;
    if(!assert_not_null(bin1)) return;
    if(!assert_equal_uint16(test_e2_timeout - acc_value, bin1->e.rank)) return;
    if(!assert_equal_ptr(test_e2_handler, bin1->e.handler)) return;
    acc_value += bin1->e.rank;

    event_bin_t* bin2 = bin1->next;
    if(!assert_not_null(bin2)) return;
    if(!assert_equal_uint16(test_e1_timeout - acc_value, bin2->e.rank)) return;
    if(!assert_equal_ptr(test_e1_handler, bin2->e.handler)) return;
    acc_value += bin2->e.rank;

    event_bin_t* bin3 = bin2->next;
    if(!assert_not_null(bin3)) return;
    if(!assert_equal_uint16(test_e3_timeout - acc_value, bin3->e.rank)) return;
    if(!assert_equal_ptr(test_e3_handler, bin3->e.handler)) return;
}

void testDummyHeadIsGoneAfterSomeTimeRunning(void) {
    alarm_timer_start();

    _delay_ms(TEST_E2_TIMOUT - 10);
    event_bin_t* head = alarm_queue->head;
    uint16_t new_head_rank = head->e.rank;

    if(new_head_rank == 0 || new_head_rank > TEST_E2_TIMOUT) {
        printf("ERROR\n");
    }
}

void testDummyHandlerAndE2HandlerHaveRunAfterEnoughTime(void) {
    _delay_ms(20);

    assert_equal_int(test_e2_timeout, handler_flag_value);
}



void dummy_handler(void) {
}

void e1_handler(void) {
    handler_flag_value = (int) test_e1_timeout;
}

void e2_handler(void) {
    handler_flag_value = (int) test_e2_timeout;
}

void e3_handler(void) {
    handler_flag_value = (int) test_e3_timeout;
}

