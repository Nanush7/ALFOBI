#include <msp430.h>
#include "intrinsics.h"
#include "i2c.h"
#include "display.h"
#include <timer.h>
#include <timer_hw.h>
#include <func_queue.h>
#include <gui.h>

typedef struct {
    arrow_direction_t direction;
    uint8_t height; 
    uint8_t active; /* A modo de prototipo! */
} arrow_t;

arrow_t arrows[4] = {
    {UP, 20, 1}, {LEFT, 17, 1}, {RIGHT, 17, 1}, {DOWN, 20, 1}
};

uint8_t score_array[4] = {'0', '0', '0', '0'};
uint8_t level_array[1] = {'1'};
gui_counter_t score = {4, score_array};
gui_counter_t level = {1, level_array};

void lower_arrows(void) {

    gui_increment_counter(&score, 7);
    gui_increment_counter(&level, 1);
    render_chars(score.digits, score.digit_amount, 0, 6);
    render_chars(level.digits, level.digit_amount, 0, 12);

    for (uint8_t i = 0; i < 4; i++) {
        arrow_t* arrow_ptr = arrows + i;

        if (!arrow_ptr->active)
            continue;

        clean_arrow(arrow_ptr->direction, arrow_ptr->height);
        arrow_ptr->height++;
        if (arrow_ptr->height > 127) {
            arrow_ptr->active = 0;
        } else {
            render_arrow(arrow_ptr->direction, arrow_ptr->height, 0);
        }
    }
}

int main() {
    /* Paramos el Watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    init_queue();
    init_i2c(0x3C);
    init_timer_hw();

    timer_t timer;
    init_timer(&timer, 4, lower_arrows);
    add_timer(timer);

    __enable_interrupt();

    init_gui();

    render_chars(score.digits, score.digit_amount, 0, 6);
    render_chars(level.digits, level.digit_amount, 0, 12);

    for (uint8_t i = 0; i < 4; i++) {
        arrow_t* arrow_ptr = arrows + i;
        render_arrow(arrow_ptr->direction, arrow_ptr->height, 0);
    }

    while (1) {
        while (!queue_is_empty()) {
            __disable_interrupt();
            func* callback = dequeue_from_queue();
            __enable_interrupt();
            callback();
        }
    }
    return 0;
}
