/*
 * File: debug.h
 *
 * Author: João Paulo Pizani Flor
 * Date: 2012-04-26
 *
 */
#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG_INPUT (*((volatile char *)0xFE))
#define DEBUG_OUTPUT (*((volatile char *)0xFF))


int debug_putc(char c, FILE* stream);
int debug_getc(FILE* stream);


#endif /* DEBUG_H_ */
