/*
 * File: assert_helpers.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-05-04
 *
 */
#ifndef ASSERT_HELPERS_H
#define ASSERT_HELPERS_H

#include <stdint.h>
#include <avr/pgmspace.h>


char* error_msg_different_ptr;
char* error_msg_different_uint8;
char* error_msg_different_uint16;
char* error_msg_different_str;
char* error_msg_null_ptr;
char* error_msg_not_null_ptr;


/// checks that a pointer isn't null and prints an error message if it is.
int assert_not_null(void* actual);

/// checks that a pointer is  null and prints an error message if it isn't.
int assert_is_null(void* actual);

/// compares two pointers for equality and prints an error message on failure.
int assert_equal_ptr(void* expected, void* actual);

/// compares two 8-bit ints for equality and prints an error message on failure.
int assert_equal_uint8(uint8_t expected, uint8_t actual);

/// compares two 16-bit ints for equality and prints an error message on failure.
int assert_equal_uint16(uint16_t expected, uint16_t actual);

/// compares two ints (16-bit) for equality and prints an error message on failure.
int assert_equal_int(int expected, int actual);

/// compares two null-terminated strings for equality and prints an error message on failure.
int assert_equal_str(char* expected, char* actual);


#endif /* end of include guard: ASSERT_HELPERS_H */
