#include <keyboard.h>
#include <msp430.h>
#include <stdint.h>
#include <timer_hw.h>

#define ROWS    (BIT0 | BIT3 | BIT4 | BIT5)
#define COLUMNS (BIT1 | BIT2 | BIT3 | BIT4)

uint8_t pressed_key = 17;

void init_keyboard(void) {
    P1SEL &= ~COLUMNS;
    P2SEL &= ~ROWS;

    P1SEL2 &= ~COLUMNS;
    P2SEL2 &= ~ROWS;

    P1DIR |=  COLUMNS; /* Columnas output. */
    P2DIR &= ~ROWS;    /* Filas como input */

    P1OUT &= ~COLUMNS; /* Inicializar columnas en bajo */

    P2REN |= ROWS; /* Habilitar resistencias pull-up en ROWS */
    P2OUT |= ROWS; /* Habilitar pull-up */

    P2IE  |=  ROWS; /* Habilitar interrupciones en filas. */
    P2IES |=  ROWS; /* Interrupción en flanco descendente. */
    P2IFG &= ~ROWS; /* Limpiar flags de interrupción. */

    set_timer_A1_callback(handle_keypress);
}

uint8_t get_pressed_key(void) {
    uint8_t res = pressed_key;
    pressed_key = 17;
    return res;
}

void handle_keypress(void) {
    /* Guardamos el estado de las columnas. */
    uint8_t previous_columns_state = P1OUT & COLUMNS;
    
    /* Leer filas. */
    uint8_t pressed_row;
    uint8_t mask = BIT0;
    uint8_t rows = P2IN & ROWS;
    for (uint8_t row = 0; row < 4; row++) {

        if (!(rows & mask)) {
            pressed_row = row;
            break;
        }

        if (!row) /* Tenemos que pasar del BIT0 al BIT3. */
            mask <<= 3;
        else
            mask <<= 1;
    }

    P1OUT = BIT1;
    uint8_t pressed_column;
    for (uint8_t column = 0; column < 4; column++) {

        if ((P2IN & ROWS) == 0x3C) {
            pressed_column = column;
            break;
        }
        P1OUT <<= 1;
    }

    /* Restaurar el estado de las columnas. */
    P1OUT = previous_columns_state;

    pressed_key = (pressed_row << 2) + pressed_column + 1;
}

#pragma vector=PORT2_VECTOR
__interrupt void teclado_isr(void) {
    P2IFG &= ~ROWS;
    enable_timer_A1();
}
