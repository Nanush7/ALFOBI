#ifndef _ASSERT_TEST_H_
#define _ASSERT_TEST_H_

#define DEBUG
#ifdef DEBUG

#include <msp430.h>
#define LED3 0x0003

#define ASSERT(expr){\
    if (!(expr)){\
        P2OUT |= LED3;\
        while(1){};\
    }\
}

#else
#define ASSERT(expr)
#endif /* DEBUG */

#endif /* _ASSERT_TEST_H_ */
