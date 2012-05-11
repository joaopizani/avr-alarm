/*
 * File: assert_helpers.c
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-04
 *
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "assert_helpers.h"



char* error_msg_different_ptr = ""
    "ERR: (ptr)e != (ptr)a\n"
    "\te=%p\n"
    "\ta=%p\n";

char* error_msg_different_int = ""
    "ERR: (u-int8-16)e != (u-int8-16)a\n"
    "\te=%d\n"
    "\ta=%d\n";

char* error_msg_different_str = ""
    "ERR: (str)e != (str)a\n"
    "\te=%s\n"
    "\ta=%s\n";

char* error_msg_null_ptr = ""
    "ERR: a pointer is unexpectedly null.\n";

char* error_msg_not_null_ptr = ""
    "ERR: a pointer is unexpectedly non-null.\n";


int assert_equal_ptr(void* expected, void* actual) {
    if(expected != actual) {
        printf(error_msg_different_ptr, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_uint8(uint8_t expected, uint8_t actual) {
    if(expected != actual) {
        printf(error_msg_different_int, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_uint16(uint16_t expected, uint16_t actual) {
    if(expected != actual) {
        printf(error_msg_different_int, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_int(int expected, int actual) {
    if(expected != actual) {
        printf(error_msg_different_int, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_str(char* expected, char* actual) {
    return 0; //TODO
}

int assert_not_null(void* actual) {
    if(actual == NULL) {
        printf(error_msg_null_ptr);
        return 0;
    }

    return 1;
}

int assert_is_null(void* actual) {
    if(actual != NULL) {
        printf(error_msg_not_null_ptr);
        return 0;
    }

    return 1;
}

