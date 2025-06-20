/**
 * @file msg_queue.h 
 *
 * @brief Módulo de cola circular de mensajes.
 */
#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdint.h>

#define MSG_QUEUE_SIZE 50

/**
 * @brief Inicializar cola de mensajes.
 */
void init_msg_queue(void);

/**
 * @brief Agregar un mensaje a la cola.
 *
 * @param msg Mensaje a agregar.
 * @pre La cola no debe estar llena.
 */
void add_to_msg_queue(uint8_t msg);

/**
 * @brief Verificar si la cola está vacía.
 *
 * @return 1 si la cola está vacía, 0 en caso contrario.
 */
uint8_t msg_queue_is_empty(void);

/**
 * @brief Verificar si la cola está llena.
 *
 * @return 1 si la cola está llena, 0 en caso contrario.
 */
uint8_t msg_queue_is_full(void);

/**
 * @brief Pasar al siguiente mensaje de la cola.
 *
 * @pre La cola no debe estar vacía.
 */
void dequeue_from_msg_queue(void);

/**
 * @brief Obtiene el siguiente mensaje de la cola sin eliminarlo.
 *
 * @return El siguiente mensaje de la cola.
 * @pre La cola no debe estar vacía.
 */
uint8_t next_from_msg_queue(void);

#endif /* MSG_QUEUE_H */
