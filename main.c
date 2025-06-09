#include <msp430.h>
#include "intrinsics.h"
#include "i2c.h"
#include <timer.h>
#include <timer_hw.h>
#include <func_queue.h>
#include <gui.h>
#include <keyboard.h>
#include <random.h>

/** TODO: Mejorar generación de números random. */
uint8_t random_byte = 0;

int main() {
    /* Paramos el Watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    /** TODO: Cambiar nombre para saber qué cola es. */
    init_queue();
    init_i2c(0x3C);
    init_keyboard();
    init_timer_hw();
    init_random(&random_byte);

    __enable_interrupt();


    // render_chars(score.digits, score.digit_amount, 0, 6);
    // render_chars(level.digits, level.digit_amount, 0, 12);

    // for (uint8_t i = 0; i < 4; i++) {
    //     arrow_t* arrow_ptr = arrows + i;
    //     render_arrow(arrow_ptr->direction, arrow_ptr->height, 0);
    // }

    while (1) {
        while (!queue_is_empty()) {
            __disable_interrupt();
            func* callback = dequeue_from_queue();
            __enable_interrupt();
            callback();
            random_byte++;
        }
        random_byte++;
    }
    return 0;
}
