#include <keyboard.h>
#include <msp430.h>
#include <stdint.h>
#include <timer_hw.h>

#define FILAS (BIT3 | BIT4 | BIT5 | BIT6)
#define COLS  (BIT0 | BIT1 | BIT2 | BIT3)

uint8_t pressed_key = 17;

void init_keyboard(void) {
    P1SEL &= ~COLS;
    P2SEL &= ~FILAS;

    P1SEL2 &= ~COLS;
    P2SEL2 &= ~FILAS;

    P1DIR |=  COLS;   // Pongo columnas como output
    P2DIR &= ~FILAS; // Pongo filas como input

    P1OUT &= ~COLS; // Inicializar columnas en bajo

    P2REN |= FILAS; // Habilitar resistencias pull-up en filas
    P2OUT |= FILAS; // Habilitar pull-up

    // Habilitar interrupciones en filas
    P2IE  |=  FILAS;
    P2IES |=  FILAS; // Interrupción en flanco descendente
    P2IFG &= ~FILAS; // Limpiar banderas de interrupción

    set_timer_A1_callback(handle_keypress);
}

uint8_t get_pressed_key(void) {
    uint8_t res = pressed_key;
    pressed_key = 17;
    return res;
}

void handle_keypress(void) {
    /* Guardamos el estado de las columnas */
    uint8_t previous_columns_state = P1OUT & COLS;
    
    /* Leer filas */
    uint8_t pressed_row;
    uint8_t mask = BIT3;
    uint8_t rows = P2IN & FILAS;
    for (uint8_t row = 0; row < 4; row++) {

        if (!(rows & mask)) {
            pressed_row = row;
            break;
        }
        mask <<= 1;
    }

    P1OUT = BIT0;
    uint8_t pressed_column;
    for (uint8_t column = 0; column < 4; column++) {

        if ((P2IN & FILAS) == 0x3C) {
            pressed_column = column;
            break;
        }
        P1OUT <<= 1;
    }

    /* Restaurar el estado de las columnas. */
    /** TODO: Probar a dejarlas todas prendidas. */
    P1OUT = previous_columns_state;

    pressed_key = (pressed_row << 2) + pressed_column + 1;
}

#pragma vector=PORT2_VECTOR
__interrupt void teclado_isr(void) {
    P2IFG &= ~FILAS;
    enable_timer_A1();
}
