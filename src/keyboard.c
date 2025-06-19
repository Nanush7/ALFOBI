#include <keyboard.h>
#include <assert_test.h>
#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include <timer_hw.h>

#define ROWS    (BIT0 | BIT3 | BIT4 | BIT5)
#define COLUMNS (BIT1 | BIT2 | BIT3 | BIT4)

const keys_t no_keys = {0};
keys_t pressed_keys = {0};

/**
 * @brief Habilita interrupciones de GPIO.
 */
void enable_interrupt_gpio(void) {
    P2IE |= ROWS;
}

/**
 * @brief Deshabilita interrupciones de GPIO.
 */
void disable_interrupt_gpio(void) {
    P2IE &= ~ROWS;
}

void init_keyboard(void) {
    set_timer_A1_callback(handle_keypress);

    P1SEL  &= ~COLUMNS;
    P1SEL2 &= ~COLUMNS;

    P2SEL  &= ~ROWS;
    P2SEL2 &= ~ROWS;

    P1DIR |= COLUMNS;        /* Columnas output. */
    P2DIR &= ~ROWS;          /* Filas input. */

    P1OUT &= ~COLUMNS;       /* Inicializar columnas en bajo. */

    P2REN |= ROWS;           /* Habilitar resistencias pull-up en ROWS */
    P2OUT |= ROWS;

    P2IES |=  ROWS;          /* Interrupción en flanco descendente. */
    P2IFG &= ~ROWS;          /* Limpiar flags de interrupción. */
    enable_interrupt_gpio(); /* Habilitar interrupciones en filas. */
}

keys_t get_pressed_keys(void) {
    keys_t res = pressed_keys;
    pressed_keys = no_keys;
    return res;
}

void handle_keypress(void) {
    /* Omitimos si entramos por rebote. */
    if (!(~P2IN & ROWS & P2IFG)) {
        P2IFG &= ~ROWS;
        enable_interrupt_gpio();
        return;
    }

    uint8_t previous_ifg = P2IFG;

    /* Casteamos a entero para poder acceder a los bits por posición, sin nombres. */
    /* Se asume que los bits están bien alineados. */
    uint16_t* pressed_keys_bits = (uint16_t*)&pressed_keys;

    /* Generamos una máscara para las filas que nos interesan (las que están bajas). */
    uint8_t target_rows_mask = ~(P2IN & ROWS);

    /* Utilizaremos los bits de checked_rows_mask para marcar a qué filas hay que limpiarles la IFG. */
    uint8_t checked_rows_mask = 0;

    /* Ahora vamos por las columnas viendo si al prenderlas se cambian las filas de la máscara. */
    uint8_t current_column_mask = BIT1;
    for (uint8_t column_index = 0; column_index < 4; column_index++) {

        P1OUT |= current_column_mask;
        uint8_t current_row_mask = BIT0;
        uint8_t active_rows = P2IN & ROWS;
        for (uint8_t row_index = 0; row_index < 4; row_index++) {

            if (active_rows & target_rows_mask & current_row_mask & previous_ifg) {
                *pressed_keys_bits |= (1 << ((row_index << 2) + column_index));
                checked_rows_mask |= current_row_mask;
            }

            /* La primera vez tenemos que pasar del BIT0 al BIT3. */
            current_row_mask <<= row_index ? 1 : 3;
        }

        P1OUT &= ~current_column_mask;
        current_column_mask <<= 1;
    }

    /* Solo limpiamos las flags de las filas procesadas y las que ya estaban apagadas. */
    P2IFG &= ~checked_rows_mask & previous_ifg;
    enable_interrupt_gpio();
}

#pragma vector=PORT2_VECTOR
__interrupt void teclado_isr(void) {
    /* No queremos que interrumpa más de una vez mientras se espera por el rebote. */
    disable_interrupt_gpio();
    enable_timer_A1();
    __low_power_mode_off_on_exit();
}
