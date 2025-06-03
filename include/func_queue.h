/**
 * @file func_queue.h
 * @brief Modulo de cola de funciones.
 */
#ifndef FUNC_QUEUE_H
#define FUNC_QUEUE_H

#include <stdint.h>

 /** Cantidad máxima de funciones que pueden haber en la cola. */
#define QUEUE_SIZE 15

typedef void (func)(void);

/**
 * @brief Inicializa la cola de funciones.
 *
 * @param
 */
void init_queue();

/**
 * @brief Agrega una función a la cola.
 *
 * @param funcptr Un puntero a una función sin retorno y sin parámetros.
 *
 * @pre La cola no debe estar llena.
 */
void add_to_queue(func* funcptr);

/**
 * @brief Pregunta si la cola esta vacia
 *
 * @retval Retorna 1 si la cola esta va. Retorna 0 si la cola no esta llena.
 */
uint8_t queue_is_empty();

/**
 * @brief Pregunta si la cola esta llena
 *
 * @retval Retorna 1 si la cola esta llena. Retorna 0 si la cola no esta llena.
 */
uint8_t queue_is_full();

/**
 * @brief Retorna y remueve el siguiente elemento de la cola, en caso de haber uno.
 *
 * @pre La cola no debe estar vacía.
 *
 * @return Siguiente elemento de la cola.
 */
func* dequeue_from_queue();

#endif // FUNC_QUEUE_H
