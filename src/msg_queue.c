/**
 * @addtogroup msg_queue
 * @{
 * @file msg_queue.c
 */
#include "msg_queue.h"
#include "assert_test.h"

/** Cola circular de mensajes */
uint8_t msg_queue[MSG_QUEUE_SIZE];
/** Primer elemento de la cola (siguiente a desencolar). */
uint8_t msg_head;
/** Próximo elemento a agregar a la cola. */
uint8_t msg_tail;

void init_msg_queue(void) {
    msg_head = 0;
    msg_tail = 0;
}

void add_to_msg_queue(uint8_t msg) {
    ASSERT(!msg_queue_is_full());

    msg_queue[msg_tail] = msg;

    if (++msg_tail == MSG_QUEUE_SIZE)
        msg_tail = 0;
}

uint8_t msg_queue_is_empty(void) {
    return msg_tail == msg_head;
}

uint8_t msg_queue_is_full(void) {
    /* return (msg_tail + 1) % QUEUE_SIZE == msg_head. */
    /* Esto no lo hacemos así debido a que no contamos con operación de módulo por hardware. */
    return msg_tail + 1 == msg_head || (msg_tail == MSG_QUEUE_SIZE - 1 && msg_head == 0);
}

void dequeue_from_msg_queue(void) {
    ASSERT(!msg_queue_is_empty());

    if (++msg_head == MSG_QUEUE_SIZE)
        msg_head = 0;
}

uint8_t next_from_msg_queue(void) {
    ASSERT(!msg_queue_is_empty());

    return msg_queue[msg_head];
}

/** @} */
