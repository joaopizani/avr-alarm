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
#define TEST_E1_ARG 1
#define TEST_E2_ARG 2
#define TEST_E3_ARG 3

uint16_t test_e2_timeout = TEST_E2_TIMOUT;
uint16_t test_e1_timeout = TEST_E1_TIMOUT;
uint16_t test_e3_timeout = TEST_E3_TIMOUT;
uint16_t test_e1_arg = TEST_E1_ARG;
uint16_t test_e2_arg = TEST_E2_ARG;
uint16_t test_e3_arg = TEST_E3_ARG;

int handler_flag_value = 0;


void testAlarmInitButNotStartHasZeroedHead(void) {
    alarm_init();

    // guarantee that there's no zero at the handler, so that we don't reset.
    alarm_queue->head->e.handler = dummy_handler;

    if(!assert_not_null(alarm_queue)) return;

    event_t* head = relative_queue_head(alarm_queue);

    if(!assert_equal_uint16(0, head->rank)) return;
}

void testAlarmWithoutInsertingAnythingGetNextOfHead(void) {
    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* after_head = head->next;
    if(!assert_is_null(after_head)) return;
}

void testFollowerOfHeadAfterInsertingOne(void) {
    int* e1_arg = malloc(sizeof(int));
    *e1_arg = test_e1_arg;
    alarm_insert(test_e1_timeout, assign_handler, e1_arg);

    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    event_bin_t* follow_head = head->next;
    if(!assert_not_null(follow_head)) return;

    if(!assert_equal_uint16(test_e1_timeout, follow_head->e.rank)) return;
    if(!assert_equal_ptr(assign_handler, follow_head->e.handler)) return;
    assert_equal_ptr(e1_arg, follow_head->e.arg_ptr);
}

void testAlarmInsertionOfThreeEvents(void) {
    int* e2_arg = malloc(sizeof(int));
    *e2_arg = test_e2_arg;
    alarm_insert(test_e2_timeout, assign_handler, e2_arg);

    int* e3_arg = malloc(sizeof(int));
    *e3_arg = test_e3_arg;
    alarm_insert(test_e3_timeout, assign_handler, e3_arg);

    event_bin_t* head = alarm_queue->head;
    if(!assert_not_null(head)) return;

    uint16_t acc_value = 0;

    event_bin_t* bin1 = head->next;
    if(!assert_not_null(bin1)) return;
    if(!assert_equal_uint16(test_e2_timeout - acc_value, bin1->e.rank)) return;
    if(!assert_equal_ptr(assign_handler, bin1->e.handler)) return;
    if(!assert_equal_ptr(e2_arg, bin1->e.arg_ptr)) return;
    acc_value += bin1->e.rank;

    event_bin_t* bin2 = bin1->next;
    if(!assert_not_null(bin2)) return;
    if(!assert_equal_uint16(test_e1_timeout - acc_value, bin2->e.rank)) return;
    if(!assert_equal_ptr(assign_handler, bin2->e.handler)) return;
    acc_value += bin2->e.rank;

    event_bin_t* bin3 = bin2->next;
    if(!assert_not_null(bin3)) return;
    if(!assert_equal_uint16(test_e3_timeout - acc_value, bin3->e.rank)) return;
    if(!assert_equal_ptr(assign_handler, bin3->e.handler)) return;
    assert_equal_ptr(e3_arg, bin3->e.arg_ptr);
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

    assert_equal_int(test_e2_arg, handler_flag_value);
}

void testGetRankOfLastValueReturnsCloseToTheRankOfE3(void) {
    uint16_t rankSum = rank_sum(alarm_queue);

    if(rankSum > ((TEST_E3_TIMOUT - TEST_E1_TIMOUT) + alarm_queue->head->e.rank)
    || rankSum < 3) {
        printf("ERROR: RankSum\n");
    }
}



void dummy_handler(void* arg_ptr) {
}

void assign_handler(void* arg_ptr) {
    handler_flag_value = *((int*) arg_ptr);
}

