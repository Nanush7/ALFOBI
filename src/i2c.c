#include "msg_queue.h"
#include "msp430g2553.h"
#include <i2c.h>
#include <msp430.h>
#include <assert_test.h>
#include "intrinsics.h"
#include <stdint.h>

#define CONTROL_DATA 0xC0

uint8_t first_sent;
uint8_t sending_data;
uint8_t control_sent;

void init_i2c(uint8_t slave_addr) {
    init_msg_queue();

    first_sent   = 0;
    sending_data = 0;
    control_sent = 0;

    /* Configuramos los pines. P1.6 como canal de clock y P1.7 como canal de data. */
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    /* Pausamos la USCI. */
    UCB0CTL1 |= UCSWRST;

    /* Se usa DCO como fuente de SMCLK por defecto */
    /* Configuramos la USCI_B: */
    /* Modo master + modo I2C + comunicación sincronizada. */
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    /* Seleccionamos SMCLK como fuente para la USCI. */
    /* DCO está a ~14 MHz, dividimos entre 35 para llegar a ~400 kHz. */
    UCB0CTL1 |= UCSSEL_2;
    UCB0BR0 = 35;
    UCB0BR1 = 0;

    /* Guardamos la dirección del esclavo. */
    UCB0I2CSA = slave_addr;

    /* Liberamos la USCI. */
    UCB0CTL1 &= ~UCSWRST;

    /* Habilitamos interrupciones. */
    IE2 |= UCB0TXIE;
}

void send_message(i2c_msg_t message) {
    /* Descartamos mensaje si la cola está llena. */
    if (msg_queue_is_full())
        return;

    /* Protegemos la cola de mensajes, ya que es un dato compartido con la ISR de transmisión. */
    __disable_interrupt();
    uint8_t was_queue_empty = msg_queue_is_empty();

    add_to_msg_queue(message);
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
        reset_i2c:
        first_sent   = 0;
        sending_data = 0;
        control_sent = 0;
        IFG2 &= ~UCB0TXIFG;
        UCB0CTL1 |= UCTXSTP;
        return;
    }

    if (first_sent && !control_sent) {
        dequeue_from_msg_queue();
    } else {
        first_sent = 1;
    }

    if (msg_queue_is_empty())
        goto reset_i2c;

    i2c_msg_t next_msg = next_from_msg_queue();
    if (sending_data && next_msg.control_byte != CONTROL_DATA) {
        first_sent   = 0;
        sending_data = 0;
        control_sent = 0;
        UCB0CTL1 |= UCTR | UCTXSTT;
        return;
    }

    if (control_sent || sending_data) {
        control_sent = 0;
        UCB0TXBUF = next_msg.data;
    } else {
        control_sent = 1;
        uint8_t continuation_bit = sending_data ? 0x80: 0x00;
        UCB0TXBUF = next_msg.control_byte | continuation_bit;
    }

    if (next_msg.control_byte == CONTROL_DATA) {
        sending_data = 1;
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void NACK_ISR(void) {
    ASSERT(UCB0STAT & UCNACKIFG);

    first_sent   = 0;
    sending_data = 0;
    control_sent = 0;
    while (UCB0CTL1 & UCTXSTP);
    /* Mandamos Restart. */
    IFG2 &= ~UCB0TXIFG;
    UCB0CTL1 |= UCTR | UCTXSTT;
}
