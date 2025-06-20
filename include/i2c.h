/**
 * @defgroup i2c
 * Módulo para transmisión I2C.
 * Se implementa un master transmitter con slave adresses de 7 bits y SMCLK como señal de clock.
 * solo se permite un slave fijo. Se utiliza el módulo @c msg_queue.h para la cola de mensajes a enviar.
 * @{
 * @file i2c.h
 */
#ifndef I2C_H
#define I2C_H

#include <stdint.h>

 /**
  * @brief Configurar la USCI_B en modo I2C.
  * Se usa como señal el SMCLK, que a su vez usa el DCO como fuente.
  *
  * @param slave_address La dirección del slave I2C.
  */
void init_i2c(uint8_t slave_address);

/**
 * @brief Enviar mensaje de 2 bytes al slave por I2C.
 *
 * @param message Arreglo con los dos bytes a enviar.
 */
void send_message(uint8_t message[2]);


#endif /* I2C_H */

/** @} */
