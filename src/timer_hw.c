/**
 * @addtogroup timer_hw
 * @{
 * @file timer_hw.c
 */
#include <msp430.h>
#include <timer.h>
#include <timer_hw.h>
#include <assert_test.h>

#define CRYSTAL_FREQ 32768
#define TA0CCR0_TARGET (CRYSTAL_FREQ / (1000 / MS_BETWEEN_TIMERA_INTERRUPTS) - 1)
#define TA1CCR0_TARGET (CRYSTAL_FREQ / (1000 / MS_BETWEEN_TIMERB_INTERRUPTS) - 1) /* Cada 10 ms. */

func* callback_timer_A1 = 0;

void disable_interrupt_timerhw(void) {
    TA0CCTL0 &= ~CCIE;
}

void enable_interrupt_timerhw(void) {
    TA0CCTL0 |= CCIE;
}

void init_timer_hw(void) {
    /* Seleccionamos el source del ACLK. */
    BCSCTL3 &= ~LFXT1S_3;
    BCSCTL3 |=  LFXT1S_0;

    /* Seteamos los TACCRx para que interrumpan después de cierta cantidad de ticks. */
    TA0CCR0 = TA0CCR0_TARGET;
    TA1CCR0 = TA1CCR0_TARGET;

    /* Activamos interrupciones de CC0. */
    enable_interrupt_timerhw();

    /*
     * TASSEL -> ACLK.
     * MC     -> Up mode.
     * Seteamos MC al final para iniciar después de configurar todos los campos.
     */

    TA0CTL &= ~TASSEL_3 & ~MC_3;
    TA0CTL |= TASSEL_1;
    TA0CTL |= MC_1 | TACLR;

    /* Dejamos el Timer_A1 apagado. */
    TA1CTL &= ~TASSEL_3 & ~MC_3;
    TA1CTL |= TASSEL_1 | TACLR;
}

void set_timer_A1_callback(func* callback) {
    callback_timer_A1 = callback;
}

void enable_timer_A1(void) {
    TA1R = 0;
    TA1CTL   |= MC_1;
    TA1CCTL0 |= CCIE;
}

/**
 * @brief Deshabilitar Timer A1.
 *
 * @note Timer A1 se deshabilita en la ISR @c Timer1_A0_ISR .
 */
void disable_timer_A1(void) {
    TA1CTL   &= ~MC_3;
    TA1CCTL0 &= ~CCIE;
}

/** ISR para Timer A0. */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) {
    if (increment_counters())
        __low_power_mode_off_on_exit();
}

/** ISR para Timer A1. */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void) {
    disable_timer_A1();
    ASSERT(callback_timer_A1); /* Si Timer_A1 está prendido, el callback no puede ser NULL. */
    add_to_queue(callback_timer_A1);
}

/** @} */
