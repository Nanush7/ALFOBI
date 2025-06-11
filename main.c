#include <msp430.h>
#include "intrinsics.h"
#include "i2c.h"
#include <timer.h>
#include <timer_hw.h>
#include <func_queue.h>
#include <gui.h>
#include <keyboard.h>
#include <random.h>
#include <game.h>

/** TODO: Mejorar generación de números random. */
uint8_t random_byte = 0;

int main() {
    /* Paramos el Watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    // BCSCTL1 &= ~RSEL1;
    BCSCTL1 |= RSEL3 | RSEL2 | RSEL1 | RSEL0;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    init_queue(); /* Cola de funciones. */
    init_i2c(0x3C);
    init_keyboard();
    init_timer_hw();
    init_random(&random_byte);
    init_gui();
    init_game();

    __enable_interrupt();

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
