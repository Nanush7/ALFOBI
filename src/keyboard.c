#include "assert_test.h"
#include "intrinsics.h"
#include "msp430g2553.h"
#include <keyboard.h>
#include <msp430.h>
#include <stdint.h>
#include <timer_hw.h>

#define ROWS    (BIT0 | BIT3 | BIT4 | BIT5)
#define COLUMNS (BIT1 | BIT2 | BIT3 | BIT4)

uint8_t pressed_key = 17;

/**
 * @brief Habilita interrupciones de GPIO.
 */
void enable_interrupt_gpio() {
    P2IE |= ROWS;
}

/**
 * @brief Deshabilita interrupciones de GPIO.
 */
void disable_interrupt_gpio() {
    P2IE &= ~ROWS;
}

void init_keyboard(void) {
    P1SEL  &= ~COLUMNS;
    P1SEL2 &= ~COLUMNS;

    P2SEL  &= ~ROWS;
    P2SEL2 &= ~ROWS;

    P1DIR |= COLUMNS; /* Columnas output. */
    P2DIR &= ~ROWS;   /* Filas input. */

    P1OUT &= ~COLUMNS; /* Inicializar columnas en bajo. */

    P2REN |= ROWS; /* Habilitar resistencias pull-up en ROWS */
    P2OUT |= ROWS;

    P2IES |=  ROWS;          /* Interrupción en flanco descendente. */
    P2IFG &= ~ROWS;          /* Limpiar flags de interrupción. */
    enable_interrupt_gpio(); /* Habilitar interrupciones en filas. */

    set_timer_A1_callback(handle_keypress);
}

uint8_t get_pressed_key(void) {
    uint8_t res = pressed_key;
    pressed_key = 17;
    return res;
}

void handle_keypress(void) {

    /* Leer filas. */
    uint8_t pressed_row = 5;
    uint8_t mask = BIT0;
    uint8_t rows = P2IN & ROWS;
    for (uint8_t row = 0; row < 4; row++) {

        if (!(rows & mask)) {
            pressed_row = row;
            break;
        }

        /* Tenemos que pasar del BIT0 al BIT3. */
        mask <<= row ? 1 : 3;
    }

    mask = BIT1;
    uint8_t pressed_column = 5;
    for (uint8_t keyboard_column = 0; keyboard_column < 4; keyboard_column++) {

        P1OUT |= mask;
        if ((P2IN & ROWS) == ROWS) {
            pressed_column = keyboard_column;
            break;
        }
        P1OUT &= ~mask;
        mask <<= 1;
    }

    /* Apagamos las columnas. */
    P1OUT &= ~COLUMNS;

    pressed_key = (pressed_row << 2) + pressed_column + 1;

    /* Limpiamos las interrupciones pendientes. */
    P2IFG &= ~ROWS;
    enable_interrupt_gpio();
}

#pragma vector=PORT2_VECTOR
__interrupt void teclado_isr(void) {
    /* No queremos que interrumpa más de una vez mientras se espera por el rebote. */
    disable_interrupt_gpio();
    P2IFG &= ~ROWS;
    enable_timer_A1();
}
