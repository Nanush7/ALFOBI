/**
 * @defgroup func_queue
 * Módulo de cola circular de funciones.
 * @{
 * @file func_queue.h
 */
#ifndef FUNC_QUEUE_H
#define FUNC_QUEUE_H

#include <stdint.h>

 /** Cantidad máxima de funciones que pueden haber en la cola. */
#define QUEUE_SIZE 15

/** Puntero a función sin parámetros y sin retorno. */
typedef void (func)(void);

/**
 * @brief Inicializar la cola de funciones.
 */
void init_queue(void);

/**
 * @brief Agregar una función a la cola.
 *
 * @param funcptr Puntero a la función a agregar.
 *
 * @pre La cola no debe estar llena.
 */
void add_to_queue(func* funcptr);

/**
 * @brief Preguntar si la cola esta vacia
 *
 * @retval Retorna 1 si la cola esta vacía y 0 en caso contrario.
 */
uint8_t queue_is_empty(void);

/**
 * @brief Preguntar si la cola está llena.
 *
 * @retval Retorna 1 si la cola está llena y 0 en caso contrario.
 */
uint8_t queue_is_full(void);

/**
 * @brief Retornar y remover el siguiente elemento de la cola.
 *
 * @return Siguiente elemento de la cola.
 * @pre La cola no debe estar vacía.
 */
func* dequeue_from_queue(void);

#endif /* FUNC_QUEUE_H */

/** @} */
