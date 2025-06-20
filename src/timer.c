/**
 * @addtogroup timer
 * @{
 * @file timer.c
 */
#include <timer.h>
#include <timer_hw.h>
#include <msp430.h>
#include <func_queue.h>
#include <assert_test.h>

/** Arreglo de timers registrados. */
timer_t timers[MAX_TIMERS_AMOUNT];
/** Tope del arreglo de timers. */
uint8_t timers_tail = 0;

void add_timer(timer_t timer) {
    ASSERT(timers_tail < MAX_TIMERS_AMOUNT);

    timers[timers_tail] = timer;
    timers_tail++;
}

void init_timer(timer_t* timer, uint8_t target, func* callback) {
    timer->target   = target;
    timer->counter  = target;
    timer->callback = callback;
}

uint8_t increment_counters(void) {
    uint8_t res = 0;
    timer_t* timer = timers;

    for (uint8_t i = timers_tail; i; i--) {

        if (!--timer->counter) {
            timer->counter = timer->target;
            add_to_queue(timer->callback);
            res = 1;
        }

        timer++;
    }

    return res;
}

/** @} */
