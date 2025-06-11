#include <msp430.h>
#include "intrinsics.h"
#include "i2c.h"
#include <timer.h>
#include <timer_hw.h>
#include <func_queue.h>
#include <display.h>
#include <keyboard.h>
#include <random.h>
#include <game.h>

/** TODO: Mejorar generación de números random. */
uint8_t random_byte = 0;

int main() {
    /* Paramos el Watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    /* Le subimos la velocidad al master clock. */
    BCSCTL1 &= ~RSEL2;
    BCSCTL1 |= RSEL3 | RSEL1 | RSEL0;

    /* LED assert. */
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    init_queue(); /* Cola de funciones. */
    init_i2c(0x3C);
    init_keyboard();
    init_timer_hw();
    init_random(&random_byte);
    init_display();

    timer_t timer_game_tick;
    init_timer(&timer_game_tick, 1, game_tick);
    add_timer(timer_game_tick);

    main_menu();

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
