#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <gui.h>
#include <timer.h>
#include <game_data.h>

typedef enum {
    MAIN,
    GAME,
    SCOREBOARD
} screen_t;

typedef enum {
    NONE,
    SINGLE,
    DOUBLE,
    TRIPLE,
    QUAD,
    REPEAT_SINGLE,
    REPEAT_DOUBLE
} sequence_mode_t;

/**
 * @brief Inicializa los timers y el estado del juego.
 */
void init_game(void);

/**
 * @brief Bajar las flechas que corresponda.
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

/** TODO: El nombre es porque tiene conflicto con el de timer.h.
 * @brief Incrementa contador según el valor pasado por parámetro.
 *
 * @param counter El contador a modificar.
 * @param value   El valor a sumar.
 *
 * @warning Hay overflow, pasa a cero.
 */
void increment_counter(gui_counter_t* counter, uint8_t value);

#endif /* GAME_H */
