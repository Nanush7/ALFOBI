/**
 * @file keyboard.h
 * @brief Módulo de configuracion y uso del teclado GPIO.
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

/** Estructura de teclas presionadas. Un bit para cada posible tecla que vale 1 si está presionada o 0 en caso contrario. */
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

/**
 * @brief Inicializar teclado matricial.
 * Inicializa puertos e interrupciones de GPIO.
 */
void init_keyboard(void);

/**
 * @brief Obtener las teclas presionadas desde la última consulta.
 *
 * @returns Struct donde las teclas presionadas están en 1.
 * @note Todas las teclas pasan a 0 luego de ejecutar esta función.
 */
keys_t get_pressed_keys(void);

/**
 * @brief Chequear y guardar las teclas presionadas.
 * Las teclas presionadas se mantienen guardadas hasta que se ejecuta @c get_pressed_keys()
 * (se van acumulando las teclas presionadas hasta que otro módulo las lea).
 */
void handle_keypress(void);

#endif /* KEYBOARD_H */
