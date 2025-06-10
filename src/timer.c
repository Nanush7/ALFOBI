#include <timer.h>
#include <timer_hw.h>
#include <msp430.h>
#include <func_queue.h>
#include <assert_test.h>

#define MAX_TIMERS_AMOUNT 5

timer_t timers[MAX_TIMERS_AMOUNT];
uint8_t timers_tail = 0;

void add_timer(timer_t timer) {
    ASSERT(timers_tail < MAX_TIMERS_AMOUNT);

    timers[timers_tail] = timer;
    timers_tail++;
}

void init_timer(timer_t* timer, uint8_t target, func* callback) {
    timer->target   = target;
    timer->counter  = 0;
    timer->callback = callback;
}

void increment_counters(void) {
    timer_t* timer = timers;
    for (uint8_t i = timers_tail; i; i--) {

        /** TODO: Esto se podría invertir, setear en target y restar hasta 0. Es más eficiente en los MSP430. */
        if (++timer->counter == timer->target) {
            timer->counter = 0;
            add_to_queue(timer->callback);
        }

        timer++;
    }
}
