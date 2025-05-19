#include "msp430g2553.h"
#include <i2c.h>
#include <msp430.h>
#include <assert_test.h>
#include <msg_queue.h>
#include "intrinsics.h"
#include <stdint.h>

uint8_t start_sent;

void init_i2c(uint8_t slave_addr) {
    init_msg_queue();

    start_sent = 0;

    /* Configuramos los pines. P1.6 como canal de clock y P1.7 como canal de data. */
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    /* Pausamos la USCI. */
    UCB0CTL1 |= UCSWRST;

    /* Se usa DCO como fuente de SMCLK por defecto */
    /* Configuramos DCO con una frecuencia típica de 0.30MHz */
    /* DCO = 3, RSEL = 3, MOD = 0 */
    /* BCSCTL1 |= RSEL1 | RSEL2; */
    /** TODO: Ver si podemos usar más de 100kHz. Tendríamos que cambiar la config del display también.
    DCOCTL  |= DCO1  | DCO2; */

    /* Configuramos la USCI_B: */
    /* Modo master + modo I2C + comunicación sincronizada. */
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    /* Seleccionamos SMCLK como fuente para la USCI a 100kHz */
    UCB0CTL1 |= UCSSEL_2;
    UCB0BR0 = 12;
    UCB0BR1 = 0;

    /* Guardamos la dirección del esclavo. */
    UCB0I2CSA = slave_addr;

    /* Liberamos la USCI. */
    UCB0CTL1 &= ~UCSWRST;

    /* Habilitamos interrupciones. */
    IE2 |= UCB0TXIE;
}

void send_message(uint8_t message[2]) {
    uint8_t was_queue_empty = msg_queue_is_empty();

    /** TODO: ver si funciona el deshabilitar solo interrupciones de la USCI */
    /* Protegemos la cola de mensajes, ya que es un dato compartido con la ISR de transmisión. */
    __disable_interrupt();
    add_to_msg_queue(message[0]);
    add_to_msg_queue(message[1]);
    __enable_interrupt();

    if (!was_queue_empty) return;

    /* Esperamos a que no haya STOP pendiente y enviamos START en modo de transmisión. */
    while (UCB0CTL1 & UCTXSTP);
    UCB0CTL1 |= UCTR | UCTXSTT;
}

/* Rutina de atención de la interrupción de buffer vacío */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void FREE_TX_BUFFER(void) {
    /* Si no tenemos más mensajes para enviar, desactivamos estas interrupciones. */
    if (msg_queue_is_empty()) {
        // IE2 &= ~UCB0TXIE;
        IFG2 &= ~UCB0TXIFG;
        return;
    }

    if (is_start_of_message() && !start_sent) {
        UCB0CTL1 |= UCTXSTT;
        start_sent = 1;
    }

    uint8_t next_byte = read_byte_from_stream();
    increment_cursor(); /* Asumimos que el stop se recibe correctamente, por lo que está bien pasar a la siguiente palabra luego de mandar el STOP. */
    if (next_byte == '\0') {
        start_sent = 0;
        if (stream_is_empty()) {
            UCB0CTL1 |= UCTXSTP;
            IFG2 &= ~UCB0TXIFG;
        }
    } else {
        UCB0TXBUF = next_byte;
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void NACK_ISR(void) {
    ASSERT(UCB0STAT & UCNACKIFG);
    start_sent = 0;
    /* El cursor vuelve al principio del mensaje fallido. */
    reset_cursor();
    /** TODO: Asumimos que se ejecuta inmediatamente la ISR de buffer vacío. */
    /** TODO: No parece funcionar esta ISR, testear. */
}
