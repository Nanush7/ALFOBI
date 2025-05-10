#include <i2c.h>
#include <msp430.h>
#include <assert_test.h>

/********************************************
 * Implementación de stream de envío I2C.   *
 * Privada porque solo se usa internamente. *
 ********************************************/

#define SEND_STREAM_SIZE 50

/* Stream circular de bytes. */
uint8_t stream[SEND_STREAM_SIZE];
/* Primer byte del stream. */
uint8_t stream_start;
/* Próximo byte a agregar en el stream. */
uint8_t stream_end;
/* Próximo byte a leer. */
uint8_t cursor;

void init_stream() {
    stream_start = 0;
    stream_end = 0;
    cursor = 0;
}

uint8_t stream_free_space() {
    if (end == start)
        return SEND_STREAM_SIZE - 1;
    else if (end > start)
        return SEND_STREAM_SIZE - 1 - (end - start);
    else
        return start - end - 1;

    /** TODO: borrar */
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 ..
    // x x x x x x x x x x d  d  d
    // e                   s

    // 0 1 2 3 4 5 6 7 8 9 10 11 12
    // d d d d d d d d x x x  x  x
    // s               e
}

void add_to_stream(uint8_t slave_address, uint8_t* msg) {
    ASSERT(1 + strlen(msg) < free_space());

    stream[stream_end] = slave_address;
    strcpy(stream[stream_end + 1], msg);

    stream_end += 1 + strlen(msg);
    if (stream_end > stream_SIZE)
        stream_end = 0;
}

uint8_t stream_is_empty() {
    return stream_end == stream_start;
}

void dequeue_from_stream(uint8_t* res) { /** TODO: cambiar el nombre de esta función */
    ASSERT(!stream_is_empty());

    strcpy(res, stream[stream_start]);

    if (++stream_start == stream_SIZE)
        stream_start = 0;
}

void init_i2c() {
    /* Seleccionamos LFXT1 como source del SMCLK */
    BCSCTL2 |= SELS;

    /* Se usa DCO como fuente de SMCLK por defecto */
    /* Configuramos DCO con una frecuencia típica de 0.30MHz */
    /* DCO = 3, RSEL = 3, MOD = 0 */
    BCSCTL1 |= RSEL1 | RSEL2;
    DCOCTL  |= DCO1 | DCO2;

    /* 1. Pausamos la lógica de la USCI */
    UCB0CTL1 |= UCSWRST;

    /* Configuramos la USCI_B en modo I2C */
    UCB0CTL0 |= UCMODE_3;

    /* 2.1 Seleccionamos SMCLK como fuente para la USCI */
    /* 2.2 Configuramos la USCI en modo master transmitter. */
    UCB0CTL1 |= UCSSEL_2 | UCTR;

    /* 3. Configuramos los pines. P1.6 como canal de clock y P1.7 como canal de data. */
    P1SEL  |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    /* 4. Liberamos la USCI. */
    UCB0CTL1 &= ~UCSWRST;
}

void send_message(uint8_t slave_address, uint8_t* message) {
    add_to_stream()    
}
