#include <timer.h>
#include <timer_hw.h>
#include <msp430.h>
#include <func_queue.h>

void init_timer(timer_t* timer, uint8_t target, func* callback) {
    timer->target   = target;
    timer->counter  = 0;
    timer->callback = callback;
}

void increment_counter(timer_t* timer) {
    if (++timer->counter == timer->target) {
        timer->counter = 0;
        add_to_queue(timer->callback);
    }
}
