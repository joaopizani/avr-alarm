/*
 * File: alarm_test.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-04
 *
 */
#ifndef ALARM_TEST_H
#define ALARM_TEST_H


void testAlarmInitButNotStartHasZeroedHead(void);
void testAlarmWithoutInsertingAnythingGetNextOfHead(void);
void testFollowerOfHeadAfterInsertingOne(void);
void testAlarmInsertionOfThreeEvents(void);
void testDummyHeadIsGoneAfterSomeTimeRunning(void);
void testDummyHandlerAndE2HandlerHaveRunAfterEnoughTime(void);
void testGetRankOfLastValueReturnsCloseToTheRankOfE3(void);


void dummy_handler(void* arg_ptr);
void assign_handler(void* arg_ptr);


#endif /* end of include guard: ALARM_TEST_H */
