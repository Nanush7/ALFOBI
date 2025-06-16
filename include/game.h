#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <gui.h>

typedef enum {
    MAIN,
    GAME,
    SCOREBOARD,
    GAME_OVER
} screen_t;

/**
 * @brief Muestra el menú principal.
 * Cambia estado a MAIN.
 */
void main_menu(void);

/**
 * @brief Inicializa los timers y el estado del juego.
 */
void init_game(void);

/**r
 * @brief Bajar las flechas que corresponda.
 * Chequea si bajó la última flecha del último nivel para mostrar win.
 */
void lower_arrows(void);

/**
 * @brief Avanzar en la secuencia del juego.
 * Maneja cambio de nivel y ejecuta generador de secuencia específico.
 */
void next_sequence(void);

/**
 * @brief Pone contador en 0.
 *
 * @param counter El contador a modificar.
 */
void reset_counter(gui_counter_t* counter);

/**
 * @brief Incrementa contador según el valor pasado por parámetro.
 *
 * @param counter El contador a modificar.
 * @param value   El valor a sumar.
 *r
 * @warning Hay overflow, pasa a cero.
 */
void increment_counter(gui_counter_t* counter, uint8_t value);

/**
 * @brief Decrementa contador según el valor pasado por parámetro.
 *
 * @param counter El contador a modificar.
 * @param value   El valor a restar.
 *r
 * @warning Hay overflow, pasa a cero.
 */
void decrement_counter(gui_counter_t* counter, uint8_t value);

/**
 * @brief Nuevo tick del juego.
 * Baja flechas y actualiza juego (secuencia, dificultad, etc.)
 */
void game_tick(void);

#endif /* GAME_H */
