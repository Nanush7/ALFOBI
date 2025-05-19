#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdint.h>

#define MSG_QUEUE_SIZE 20

/** TODO: Agregar doxygen. */
void init_msg_queue(void);

void add_to_msg_queue(uint8_t msg);

uint8_t msg_queue_is_empty(void);

uint8_t msg_queue_is_full(void);

uint8_t dequeue_from_msg_queue(void);

#endif /* MSG_QUEUE_H */
