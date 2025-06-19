#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdint.h>

#define MSG_QUEUE_SIZE 50

typedef struct {
  uint8_t control_byte;
  uint8_t data;
} i2c_msg_t;

/** TODO: Agregar doxygen. */
void init_msg_queue(void);

void add_to_msg_queue(i2c_msg_t msg);

uint8_t msg_queue_is_empty(void);

uint8_t msg_queue_is_full(void);

void dequeue_from_msg_queue(void);

i2c_msg_t next_from_msg_queue(void);

#endif /* MSG_QUEUE_H */
