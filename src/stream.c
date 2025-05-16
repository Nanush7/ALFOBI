#include <stream.h>
#include <string.h>
#include <assert_test.h>

/* Stream circular de bytes. */
uint8_t stream[SEND_STREAM_SIZE];
/* Primer byte del stream. */
uint8_t stream_start;
/* Próximo byte a agregar en el stream. */
uint8_t stream_end;
/* Próximo byte a leer. */
uint8_t cursor;

void init_stream() {
    stream_start = 0;
    stream_end = 0;
    cursor = 0;
}

uint8_t stream_free_space() {
    if (stream_end == stream_start)
        return SEND_STREAM_SIZE - 1;
    else if (stream_end > stream_start)
        return SEND_STREAM_SIZE - 1 - (stream_end - stream_start);
    else
        return stream_start - stream_end - 1;

    /** TODO: borrar */
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 ..
    // x x x x x x x x x x d  d  d
    // e                   s

    // 0 1 2 3 4 5 6 7 8 9 10 11 12
    // d d d d d d d d x x x  x  x
    // s               e
}

/**
 * @brief Copia un mensaje al stream circular.
 *
 * @param msg Puntero al mensaje que se desea copiar.
 */
void circular_strcpy(uint8_t* msg) {
    uint8_t index = stream_end;
    for (int i = 0; i < strlen(msg) + 1; i++) {
        stream[index] = msg[i];
        index++;
        if (index >= SEND_STREAM_SIZE) {
            index = 0;
        }
    }
    /** TODO: Para qué está este assert? Independientemente del
     * propósito, está mal porque podría haber un \0 basura.
     *ASSERT(stream[index] == '\0'); */
}

void add_to_stream(uint8_t* msg) {
    ASSERT(strlen(msg) + 1 <= stream_free_space());  /* +1 porque hay \0 */

    circular_strcpy(msg);

    stream_end += strlen(msg) + 1;
    if (stream_end >= SEND_STREAM_SIZE)
        stream_end -= SEND_STREAM_SIZE;

    ASSERT(stream_end != stream_start);
}

uint8_t stream_is_empty(void) {
    return stream_end == stream_start;
}

uint8_t read_byte_from_stream(void) {
    ASSERT(!stream_is_empty());

    return stream[cursor];
}

void increment_cursor(void) {
    uint8_t end_of_message = stream[cursor] == '\0';

    cursor++;
    if (cursor >= SEND_STREAM_SIZE) {
        cursor = 0;
    }

    if (end_of_message) {
        stream_start = cursor;
    }
}

void reset_cursor(void) {
    cursor = stream_start;
}

uint8_t is_start_of_message(void) {
    return stream_start == cursor;
}
