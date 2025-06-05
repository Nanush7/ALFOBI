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

/* En caso de cambiarse esto, se debería modificar el código del timer_hw. */
#define MS_BETWEEN_TIMER_INTERRUPTS 50

/*
 * El timer se inicializa en 0 por defecto en el archivo de implementación.
 */

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
 * @brief Inicializa un timer recurrente.
 *
 * @param target_interval Cada cuántas interrupciones del reloj se encola la 
 */
void init_timer(timer_t* timer, uint8_t target, func* callback);

/**
 * @brief Incrementa el contador del timer. En caso de llegar a target, se resetea el contador y se ejecuta el callback.
 *
 * @param timer El timer a incrementar.
*/
void increment_counter(timer_t* timer);

#endif // TIMER_H
