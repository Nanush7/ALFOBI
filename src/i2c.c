#include <i2c.h>
#include <msp430.h>
#include <assert_test.h>
#include <stream.h>
#include <stdint.h>

uint8_t start_sent;

void init_i2c(uint8_t slave_addr) {
    start_sent = 0;
    
    /* Se usa DCO como fuente de SMCLK por defecto */
    /* Configuramos DCO con una frecuencia típica de 0.30MHz */
    /* DCO = 3, RSEL = 3, MOD = 0 */
    BCSCTL1 |= RSEL1 | RSEL2;
    DCOCTL  |= DCO1  | DCO2;

    /* 1. Pausamos la lógica de la USCI */
    UCB0CTL1 |= UCSWRST;

    /* Guardamos la dirección del esclavo. */
    UCB0I2CSA = slave_addr;

    /* Configuramos la USCI_B en modo I2C */
    /* Configuramos la comunicación sincronizada. */
    /* Configuramos la USCI_B en modo Master. */
    UCB0CTL0 |= UCMODE_3 | UCSYNC | UCMST;

    /* 2.1 Seleccionamos SMCLK como fuente para la USCI */
    /* 2.2 Configuramos la USCI en modo master transmitter. */
    UCB0CTL1 |= UCSSEL_2 | UCTR;

    /* 3. Configuramos los pines. P1.6 como canal de clock y P1.7 como canal de data. */
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;
    P1REN  |= BIT6 | BIT7;
    P1OUT  |= BIT6 | BIT7;

    P2SEL &= ~BIT1 + ~BIT2; //Colocamos P2.1 y P2.2 en la funcion reservada,
    P2SEL2 |= BIT1 + BIT2; // para que dejen pasar las señales de la placa conectada a ellos

    /* 4. Liberamos la USCI. */
    UCB0CTL1 &= ~UCSWRST;
}

void send_message(uint8_t* message) {
    uint8_t empty_stream = stream_is_empty();

    add_to_stream(message);

    if (!empty_stream) return;

    /* Activar interrupciones de buffer vacío. */
    IE2 |= UCB0TXIE;
    UCB0CTL1 |= UCTXSTT;
}

/* Rutina de atención de la interrupción de buffer vacío */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void FREE_TX_BUFFER(void) {
    /* Si no tenemos más mensajes para enviar, desactivamos estas interrupciones. */
    if (stream_is_empty()) {
        IE2 &= ~UCB0TXIE;
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
            IE2 &= ~UCB0TXIE;
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
}
