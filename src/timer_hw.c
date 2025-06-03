#include <msp430.h>
#include <timer.h>
#include <timer_hw.h>
#include <assert_test.h>

#define CRYSTAL_FREQ 32768
#define MAX_TIMERS_AMOUNT 5
#define TACCR0_TARGET (CRYSTAL_FREQ / (1000 / TIMER_INTERVAL) - 1)

timer_t timers[MAX_TIMERS_AMOUNT];
uint8_t timer_tail = 0;

void add_timer(timer_t timer) {
    ASSERT(timer_tail < MAX_TIMERS_AMOUNT);

    timers[timer_tail] = timer;
    timer_tail++;
}

void disable_interrupt_timerhw() {
    TACCTL0 &= ~CCIE;
}

void enable_interrupt_timerhw() {
    TACCTL0 |= CCIE;
}

void init_timer_hw() {
    /* Seleccionamos el source del ACLK. */
    /* Seteamos el TACCR0 para que interrumpa cada TIMER_INTERVAL ms. */
    BCSCTL3 = (BCSCTL3 & ~LFXT1S_3);
    BCSCTL3 |= LFXT1S_0;
    TACCR0 = TACCR0_TARGET;

    /* Activamos interrupciones de CC0. */
    enable_interrupt_timerhw();

    /*
     * TASSEL -> ACLK.
     * MC     -> Up mode.
     * Seteamos MC al final para iniciar después de configurar todos los campos.
     */
    TACTL &= ~TASSEL_3 & ~MC_3;
    TACTL |= TASSEL_1;
    TACTL |= MC_1;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void CC0_ISR(void) {
    for (uint8_t i = 0; i < timer_tail; i++) {
        timer_t* timer = timers + i;
        increment_counter(timer);
    }
}
