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

/**
 * @brief Inicializa el stream circular para enviar datos.
 */

void init_stream() {
    stream_start = 0;
    stream_end = 0;
    cursor = 0;
}

/**
 * @brief Calcula el espacio libre en el stream circular.
 *
 * @return El espacio disponible en bytes.
 */

uint8_t stream_free_space() {
    if (stream_end == stream_start)
        return SEND_STREAM_SIZE - 1;
    else if (stream_end > stream_start)
        return SEND_STREAM_SIZE - 1 - (stream_end - stream_start);
    else
        return stream_start - stream_end - 1;

    /** TODO: borrar */
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 ..
    // x x x x x x x x x x d  d  d
    // e                   s

    // 0 1 2 3 4 5 6 7 8 9 10 11 12
    // d d d d d d d d x x x  x  x
    // s               e
}

/**
 * @brief Copia un mensaje al stream circular.
 *
 * @param msg Puntero al mensaje que se desea copiar.
 */
void circular_strcpy(uint8_t* msg) {
    uint8_t index = stream_end;
    for (int i = 0; i < strlen(msg) + 1; i++) {
        stream[index] = msg[i];
        index++;
        if (index >= SEND_STREAM_SIZE) {
            index = 0;
        }
    }
    ASSERT(stream[index] == '\0');
}

/**
 * @brief Agrega un mensaje al stream circular.
 *
 * @param msg Puntero al mensaje que se desea agregar.
 *
 * @note El mensaje debe tener espacio suficiente en el stream.
 */

void add_to_stream(uint8_t* msg) {
    ASSERT(strlen(msg) + 1 <= stream_free_space());  /* +1 porque hay \0 */

    ciruclar_strcpy(msg);

    stream_end += 1 + strlen(msg);
    if (stream_end > stream_SIZE)
        stream_end = 0;

    ASSERT(stream_end != stream_start);
}
/**
 * @brief Verifica si el stream está vacío.
 *
 * @return 1 si el stream está vacío, 0 en caso contrario.
 */
uint8_t stream_is_empty(void) {
    return stream_end == stream_start;
}

/**
 * @brief Lee un byte del stream.
 *
 * @note No avanza el cursor.
 *
 * @return El caracter leído del stream.
 */
uint8_t read_byte_from_stream(uint8_t res) {
    ASSERT(!stream_is_empty());

    return stream[stream_start];
}

/**
* @brief Avanza el cursor en el stream circular
*
* @note Si se encuentra un caracter de fin de mensaje actualiza la posicion al inicio del stream.
*/
void increment_cursor(void) {
    uint8_t end_of_word = stream[cursor] == '\0';

    cursor++;
    if (cursor >= SEND_STREAM_SIZE) {
        cursor = 0;
    }

    if (end_of_word) {
        stream_start = cursor;
    }
}

/**
 * @brief Resetea el cursor del stream al comienzo de la palabra siendo leída.
 */
void reset_cursor(void) {
    cursor = stream_start;
}

/**********************************************
 * Fin implementación de stream de envío I2C. *
 * Privada porque solo se usa internamente.   *
 **********************************************/

void init_i2c(uint8_t slave_addr) {
    /* Guardamos la dirección del esclavo. */
    UCBB0I2CSA = slave_addr;

    /* Seleccionamos LFXT1 como source del SMCLK */
    BCSCTL2 |= SELS;

    /* Se usa DCO como fuente de SMCLK por defecto */
    /* Configuramos DCO con una frecuencia típica de 0.30MHz */
    /* DCO = 3, RSEL = 3, MOD = 0 */
    BCSCTL1 |= RSEL1 | RSEL2;
    DCOCTL  |= DCO1  | DCO2;

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

void send_message(uint8_t* message) {
    uint8_t empty_stream = stream_is_empty();

    add_to_stream(message);

    if (!empty_stream) return;

    UCB0CTL1 |= UCTXSTT;

    /* En teoría nunca debería no estar disponible, ya que solo entramos a este
     caso si nunca se transmitió o si ya se transmitieron todos los mensajes y luego se mandó un STOP, sin escribir de nuevo al buffer */
    UCB0TXBUF = slave_address;

    /* Activar interrupciones de buffer vacío. */
    IE2 |= UCB0TXIE;
}


/* Rutina de atención de la interrupción de buffer vacío */

#pragma vector=USCIAB0TX_VECTOR
__interupt void FREE_TX_BUFFER(void) {
    /* Si no tenemos más mensajes para enviar, desactivamos estas interrupciones. */
    if (stream_is_empty()) {
        IE2 &= ~UCB0TXIE;
        return;
    }

    increment_cursor();
    uint8_t next_byte = read_byte_from_stream();
    if (next_byte == '\0') {
        UCB0CTL1 |= UCTXSTP;

    }
    UCB0TXBUF = next_byte;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void NACK_ISR(void) {
    ASSERT(UCB0STAT & UCNACKIFG);

    /* Mandamos un (re)start al slave. */
    UCB0CTL1 |= UCTXSTT;
    /* El cursor vuelve al principio del mensaje fallido. */
    reset_cursor();
    /** TODO: Asumimos que se ejecuta inmediatamente la ISR de buffer vacío. */
}
