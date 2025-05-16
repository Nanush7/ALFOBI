/**
 * @brief Módulo de stream circular.
 */

#ifndef _STREAM_H_
#define _STREAM_H_

#include <stdint.h>

#define SEND_STREAM_SIZE 60

 /**
  * @brief Inicializa el stream circular.
  */

void init_stream(void);

/**
 * @brief Calcula el espacio libre en el stream circular.
 *
 * @return El espacio disponible en bytes.
 */

uint8_t stream_free_space(void);

/**
 * @brief Agrega un mensaje al stream circular.
 *
 * @param msg Puntero al mensaje que se desea agregar.
 *
 * @note El mensaje debe tener espacio suficiente en el stream.
 */

void add_to_stream(uint8_t* msg);

/**
 * @brief Verifica si el stream está vacío.
 *
 * @return 1 si el stream está vacío, 0 en caso contrario.
 */
uint8_t stream_is_empty(void);

/**
 * @brief Lee un byte del stream.
 *
 * @note No avanza el cursor.
 *
 * @return El caracter leído del stream.
 */
uint8_t read_byte_from_stream(void);

/**
* @brief Avanza el cursor en el stream circular.
*
* @note Si se encuentra un caracter de fin de mensaje, se mueve el inicio del stream al siguiente mensaje.
*/
void increment_cursor(void);

/**
 * @brief Resetea el cursor del stream al comienzo del mensaje siendo leído.
 */
void reset_cursor(void);

/**
 * @brief Verifica si el cursor se encuentra en el inicio del mensaje.
 *
 * @return Retorna 1 si el cursor se encuentra en el inicio del primer mensaje del stream, o 0 en caso contrario.
 */
uint8_t is_start_of_message(void);

#endif
