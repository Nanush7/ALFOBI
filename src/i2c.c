#include "msp430g2553.h"
#include <i2c.h>
#include <msp430.h>
#include <assert_test.h>
#include "msg_queue.h"
#include "intrinsics.h"
#include <stdint.h>

uint8_t first_sent;

void init_i2c(uint8_t slave_addr) {
    init_msg_queue();

    first_sent = 0;

    /* Configuramos los pines. P1.6 como canal de clock y P1.7 como canal de data. */
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    /* Pausamos la USCI. */
    UCB0CTL1 |= UCSWRST;

    /* Se usa DCO como fuente de SMCLK por defecto */

    /* Configuramos la USCI_B: */
    /* Modo master + modo I2C + comunicación sincronizada. */
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    /* Seleccionamos SMCLK como fuente para la USCI a 1 MHz (SMCLK a ~14 MHz / 12). */
    /* Si se quiere reducir la frecuencia de I2C, se debe cambiar la inicialización del display para */
    /* adaptarlo a la nueva frecuencia. */
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
    /* Protegemos la cola de mensajes, ya que es un dato compartido con la ISR de transmisión. */
    __disable_interrupt();
    uint8_t was_queue_empty = msg_queue_is_empty();

    add_to_msg_queue(message[0]);
    add_to_msg_queue(message[1]);
    if (!was_queue_empty) {
        __enable_interrupt();
        return;
    }
    first_sent = 0;
    __enable_interrupt();

    /* Esperamos a que no haya STOP pendiente y enviamos START en modo de transmisión. */
    while (UCB0CTL1 & UCTXSTP);
    while (UCB0STAT & UCBBUSY);
    UCB0CTL1 |= UCTR | UCTXSTT;
}

/* Rutina de atención de la interrupción de buffer vacío */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void FREE_TX_BUFFER(void) {
    /* Si no tenemos más mensajes para enviar, desactivamos estas interrupciones. */
    if (msg_queue_is_empty()) {
        first_sent = 0;
        IFG2 &= ~UCB0TXIFG;
        UCB0CTL1 |= UCTXSTP;
        return;
    }

    if (first_sent) {
        dequeue_from_msg_queue();
    } else {
        first_sent = 1;
    }

    if (!msg_queue_is_empty()) {
        UCB0TXBUF = next_from_msg_queue();
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void NACK_ISR(void) {
    ASSERT(UCB0STAT & UCNACKIFG);

    first_sent = 0;
    while (UCB0CTL1 & UCTXSTP);
    /* Mandamos Restart. */
    UCB0CTL1 |= UCTR | UCTXSTT;
}
