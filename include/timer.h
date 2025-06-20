/**
 * @file timer.h
 * @brief Proporciona funciones y una estructura para gestionar el tiempo.
 *
 * Este archivo contiene la declaración de las funciones y la estructura necesaria para
 * gestionar el tiempo en formato horas, minutos, segundos y milisegundos.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <func_queue.h>

#define MS_BETWEEN_TIMERA_INTERRUPTS 20
#define MS_BETWEEN_TIMERB_INTERRUPTS 10

/**
 * @struct timer_t
 * @brief Estructura para manejo de timers.
 *
 * @param target   valor del counter necesario para ejecutar callback.
 * @param counter  cuenta actual.
 * @param callback función a ejecutar cuando se alcanza el target.
 */
typedef struct {
    uint8_t target;
    uint8_t counter;
    func* callback;
} timer_t;

/**
 * @brief Inicializar un timer recurrente.
 *
 * @param target_interval Cada cuántas interrupciones del reloj se encola la 
 */
void init_timer(timer_t* timer, uint8_t target, func* callback);

/**
 * @brief Agregar timer lógico a la lista de timers lógicos.
 * 
 * @param timer El timer lógico a agregar.
 */
void add_timer(timer_t timer);

/**
 * @brief Incrementar los timers lógicos registrados. En caso de llegar a target, se resetea el contador y se ejecuta el callback.
 *
 * @returns 1 si algún timer llegó al target. 0 en caso contrario.
 */
uint8_t increment_counters(void);

#endif // TIMER_H
