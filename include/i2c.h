/**
 * @brief funciones para posibilitar comunicación por I2C.
 * Se implementa un master transmitter con slave adresses de 7 bits y SMCLK.
 */
#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

#define LARGO_MAXIMO_MENSAJE 17
 /**
  * LARGO_MAXIMO_BUFFER = LARGO_MAXIMO_MENSAJE -1 (el primero se manda inmediatamente) + 1 \n + 1 \0
  */
#define LARGO_MAXIMO_BUFFER LARGO_MAXIMO_MENSAJE+1

/**
 * @brief configura la USCI_B en modo I2C. 
 * Se usa como fuente de reloj el SMCLK, que a su vez usa como fuente el VLO.
 */
void init_i2c();

/**
 * @brief Inicia el envío de mensaje por I2C. 
 * @pre message debe ser un string terminado con \n
 */
void send_message(uint8_t slave_address, uint8_t* message);


#endif /* I2C_H_ */
