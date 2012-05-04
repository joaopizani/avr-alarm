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


PGM_P error_msg_different_ptr = ""
    "ERROR: (void*)expected != (void*)actual\n"
    "    expected: %p\n"
    "    actual: %p\n";

PGM_P error_msg_different_uint8 = ""
    "ERROR: (uint8_t)expected != (uint8_t)actual\n"
    "    expected: %d\n"
    "    actual: %d\n";

PGM_P error_msg_different_uint16 = ""
    "ERROR: (uint16_t)expected != (uint16_t)actual\n"
    "    expected: %d\n"
    "    actual: %d\n";

PGM_P error_msg_different_str = ""
    "ERROR: (char[])expected != (char[])actual\n"
    "    expected: %s\n"
    "    actual: %s\n";

PGM_P error_msg_null_ptr = ""
    "ERROR: a pointer is null, which should not be.\n";

PGM_P error_msg_not_null_ptr = ""
    "ERROR: a pointer, which SHOULD be null, isn't\n";


int assert_equal_ptr(void* expected, void* actual) {
    char msg_buffer[200];

    if(expected != actual) {
        strcpy_P(msg_buffer, (PGM_P) &error_msg_different_ptr);
        printf(msg_buffer, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_uint8(uint8_t expected, uint8_t actual) {
    return 0; //TODO
}

int assert_equal_uint16(uint16_t expected, uint16_t actual) {
    char msg_buffer[200];

    if(expected != actual) {
        strcpy_P(msg_buffer, (PGM_P) &error_msg_different_uint16);
        printf(msg_buffer, expected, actual);
        return 0;
    }

    return 1;
}

int assert_equal_int(int expected, int actual) {
    return 0; //TODO
}

int assert_equal_str(char* expected, char* actual) {
    return 0; //TODO
}

int assert_not_null(void* actual) {
    char msg_buffer[200];

    if(actual == NULL) {
        strcpy_P(msg_buffer, (PGM_P) &error_msg_null_ptr);
        printf(msg_buffer);
        return 0;
    }

    return 1;
}

int assert_is_null(void* actual) {
    char msg_buffer[200];

    if(actual != NULL) {
        strcpy_P(msg_buffer, (PGM_P) &error_msg_not_null_ptr);
        printf(msg_buffer);
        return 0;
    }

    return 1;
}

