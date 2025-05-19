#include "msg_queue.h"

/** Cola circular de mensajes */
uint8_t msg_queue[MSG_QUEUE_SIZE];
/** Primer elemento de la cola. */
uint8_t msg_head;
/** Próximo elemento a agregar en la cola. */
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

uint8_t dequeue_from_msg_queue(void) {
    ASSERT(!msg_queue_is_empty());

    uint8_t res = msg_queue[msg_head];

    if (++msg_head == MSG_QUEUE_SIZE)
        msg_head = 0;

    return res;
}
