#ifndef _ASSERT_TEST_H_
#define _ASSERT_TEST_H_

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

#endif /* _ASSERT_TEST_H_ */
