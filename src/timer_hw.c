#include <msp430.h>

#include <timer.h>
#include <timer_hw.h>


const unsigned int  CRYSTAL_FREQ = 32768;
const unsigned int  VLO_FREQ     = 12000;

void disable_interrupt_timerhw() {
    TACCTL0 &= ~CCIE;
}

void enable_interrupt_timerhw() {
    TACCTL0 |= CCIE;
}

void init_timer(timer_source_t source) {
    /* Seleccionamos el source de ACLK (LFXT1 o VLO). */
    /* Seteamos el TACCR0 para que interrumpa cada 250ms. */    
    BCSCTL3 = (BCSCTL3 & ~LFXT1S_3);
    switch (source) {
        case TIMER_SOURCE_CRYSTAL:
            BCSCTL3 |= LFXT1S_0;
            TACCR0 = (CRYSTAL_FREQ / 4) - 1;
            break;
        case TIMER_SOURCE_VLO:
            BCSCTL3 |= LFXT1S_2;
            TACCR0 = (VLO_FREQ / 4) - 1;
            break;
    }

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
    inc_time();
}
