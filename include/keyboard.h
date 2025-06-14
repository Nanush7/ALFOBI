/**
 * @file keyboard.h
 * @brief Configuracion y procesamiento del teclado.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

typedef struct {
    uint8_t one:   1;
    uint8_t two:   1;
    uint8_t three: 1;
    uint8_t a:     1;
    uint8_t four:  1;
    uint8_t five:  1;
    uint8_t six:   1;
    uint8_t b:     1;
    uint8_t seven: 1;
    uint8_t eight: 1;
    uint8_t nine:  1;
    uint8_t c:     1;
    uint8_t aster: 1;
    uint8_t zero:  1;
    uint8_t hash:  1;
    uint8_t d:     1;
} keys_t;

/** TODO: Acomodar Doxygen.
 * @brief Se configura el teclado para la correcta recepción de botones.
 * Se establecen los pines correspondientes a las columnas como output y a los de las filas como input con resistencia de pull up. Se habilitan las interrupciones de estas últimas.
 */

void init_keyboard(void);

/**
 * @brief Obtiene las teclas presionadas desde la última consulta.
 *
 * @return Struct que contiene las teclas presionadas en 1.
 *
 * @note Se borran los botones seleccionados al ejecutarla.
 */
keys_t get_pressed_keys(void);

/**
 * @brief Chequear y guarda los botones presionados.
 */
void handle_keypress(void);

#endif /* KEYBOARD_H */
