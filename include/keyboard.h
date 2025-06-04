/**
 * @file keyboard.h
 * @brief Configuracion y procesamiento del teclado.
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>


/** TODO: Acomodar Doxygen.
 * @brief Se configura el teclado para la correcta recepcion de botones.
 * @paragraph Se establecen los pines correspondientes a las columnas como output y a los de las filas como input con resistencia de pull up. Se habilitan las interrupciones de estas �ltimas.
 */

void init_keyboard(void);

/**
 * @brief Obtiene las teclas presionadas desde la última consulta.
 */
uint8_t get_pressed_key(void);

/**
 * @brief Se hace el procesamiento de la recepcion de un boton.
 *
 * @return Se devuelve el numero (del 1 al 16) del boton apretado.
 */
void handle_keypress(void);

#endif /* KEYBOARD_H */
