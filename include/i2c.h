/**
 * @brief funciones para posibilitar comunicación por I2C.
 * Se implementa un master transmitter con slave adresses de 7 bits y SMCLK.
 */
#ifndef I2C_H
#define I2C_H

#include <stdint.h>

 /**
  * @brief Configurar la USCI_B en modo I2C.
  * Se usa como fuente de reloj el SMCLK, que a su vez usa como fuente el VLO.
  */
void init_i2c(uint8_t slave_address);

/**
 * @brief Iniciar el envío de mensaje por I2C.
 */
void send_message(uint8_t message[2]);


#endif /* I2C_H */
