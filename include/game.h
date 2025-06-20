/**
 * @defgroup Game
 * @{
 * @file game.h
 */
#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <gui.h>

typedef enum {
    MAIN,
    GAME,
    SCOREBOARD,
    GAME_OVER,
    INFO
} screen_t;

/**
 * @brief Muestra el menú principal.
 * Cambia estado a @c MAIN .
 */
void main_menu(void);

/**
 * @brief Inicializar juego e iniciar partida.
 * Cambia estado a @c GAME .
 */
void init_game(void);

/**
 * @brief Bajar las flechas que corresponda.
 * Chequea si bajó la última flecha del último nivel y llama a @c game_over() .
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
 *
 * @warning Hay overflow. Todos los digitos pasan a cero.
 */
void increment_counter(gui_counter_t* counter, uint8_t value);

/**
 * @brief Decrementa contador según el valor pasado por parámetro.
 *
 * @param counter El contador a modificar.
 * @param value   El valor a restar.
 *
 * @warning Hay overflow. Todos los dígitos pasan a nueve.
 */
void decrement_counter(gui_counter_t* counter, uint8_t value);

/**
 * @brief Nuevo tick del juego.
 * Baja flechas, actualiza juego (secuencia, dificultad, etc.) y maneja pulsaciones del teclado.
 */
void game_tick(void);

/**
 * @brief Inicializar tabla de scores.
 * Se usa para setear el scoreboard leído de la FLASH desde main.
 *
 * @param scores_param Puntero al struct de scores.
 */
void set_scores(scores_t* scores_param);

/**
 * @brief Inicializar puntero a seed para generación de números pseudoaleatorios.
 * El valor de la seed será modificado desde main y accedido a través del puntero al iniciar una nueva partida.
 *
 * @param seed_ptr Puntero a la seed manejada por main.
 */
void init_game_seed(uint16_t* seed_ptr);

#endif /* GAME_H */

/** @} */
