/**
 * @defgroup timer
 * @{
 * @file timer.h
 * @brief Proporciona funciones y una estructura para gestionar timers lógicos con distintos targets y callbacks.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <func_queue.h>

#define MS_BETWEEN_TIMERA_INTERRUPTS 20
#define MS_BETWEEN_TIMERB_INTERRUPTS 10
#define MAX_TIMERS_AMOUNT 5

/** Estructura para manejo de timers lógicos. */
typedef struct {
    uint8_t target; /**< Valor del counter necesario para ejecutar callback. */
    uint8_t counter; /**< Cuenta actual. */
    func* callback; /**< Función a ejecutar cuando se alcanza el target. */
} timer_t;

/**
 * @brief Inicializar un timer recurrente.
 *
 * @param timer           Puntero al timer a inicializar.
 * @param target_interval Cada cuántas interrupciones del Timer A0 se encola el callback.
 * @param callback        Puntero a función callback.
 */
void init_timer(timer_t* timer, uint8_t target, func* callback);

/**
 * @brief Agregar timer lógico a la lista de timers lógicos.
 * 
 * @param timer El timer lógico a agregar.
 * @pre La cantidad de timers agregados no debe superar @c MAX_TIMERS_AMOUNT .
 */
void add_timer(timer_t timer);

/**
 * @brief Incrementar los timers lógicos registrados y manejar targets y callbacks.
 * En caso de llegar a target, se resetea el contador y se ejecuta el callback del timer correspondiente.
 *
 * @return 1 si algún timer llegó al target. 0 en caso contrario.
 */
uint8_t increment_counters(void);

#endif /* TIMER_H */

/** @} */
