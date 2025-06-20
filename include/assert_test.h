/**
 * @file assert_test.h
 */
#ifndef ASSERT_TEST_H
#define ASSERT_TEST_H

#define DEBUG
#ifdef DEBUG

#include <msp430.h>

#define ASSERT(expr){\
    if (!(expr)){\
        P1OUT |= BIT0;\
        __disable_interrupt();\
        while(1){};\
    }\
}

#else
#define ASSERT(expr)
#endif /* DEBUG */

#endif /* ASSERT_TEST_H */
