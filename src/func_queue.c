#include <stdint.h>
#include <func_queue.h>
#include <assert_test.h>

/** Cola circular */
func* queue[QUEUE_SIZE];
/** Primer elemento de la cola. */
uint8_t head;
/** Próximo elemento a agregar en la cola. */
uint8_t tail;

void init_queue(void) {
    head = 0;
    tail = 0;
}

void add_to_queue(func* funcptr) {
    ASSERT(!queue_is_full());

    queue[tail] = funcptr;

    if (++tail == QUEUE_SIZE)
        tail = 0;
}

uint8_t queue_is_empty(void) {
    return tail == head;
}

uint8_t queue_is_full(void) {
    /* return (tail + 1) % QUEUE_SIZE == head. */
    /* Esto no lo hacemos así debido a que no contamos con operación de módulo por hardware. */
    return tail + 1 == head || (tail == QUEUE_SIZE - 1 && head == 0);
}

func* dequeue_from_queue(void) {
    ASSERT(!queue_is_empty());

    func* res = queue[head];

    if (++head == QUEUE_SIZE)
        head = 0;

    return res;
}
