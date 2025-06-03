#ifndef GUI_H
#define GUI_H

#include <stdint.h>

typedef enum {
    RIGHT = 0,
    DOWN  = 1,
    UP    = 2,
    LEFT  = 3,
} arrow_direction_t;

typedef struct {
    const uint8_t* arrow_template;
    const uint8_t* outline_template;
    const arrow_direction_t arrow_direction;
    const uint8_t template_size;
    uint8_t outline_height;
    uint8_t speed;
    uint8_t page;
} global_arrow_data_t;

typedef struct {
    const uint8_t digit_amount;
    uint8_t* digits;  /* Cada dígito en una posición del array (LSF). Más eficiente para escribir en el display. */
} gui_counter_t;

struct arrow {
    uint8_t height;
    arrow_direction_t direction;
};

typedef struct {
    uint8_t start : 4;
    uint8_t end   : 4;
} template_range_t;


/**
 * @brief Inicializar.
 */
void init_gui(void);

/**
 * @brief Renderizar y enviar flecha al display.
 *
 * @param direction  La dirección de la flecha (arriba, abajo, etc.).
 * @param height     La altura actual de la flecha (entre 17 y 127 para derecha/izquiera y entre 20 y 127 para arriba/abajo).
 * @param is_outline Determina si dibujar el contorno o la flecha.
 */
void render_arrow(arrow_direction_t direction, uint8_t height, uint8_t is_outline);

/**
 * @brief Limpiar posición donde había flecha en el display.
 *
 * @param direction La dirección de la flecha (arriba, abajo, etc.).
 * @param height    La altura actual de la flecha (entre 17 y 127 para derecha/izquiera y entre 20 y 127 para arriba/abajo).
 */
void clean_arrow(arrow_direction_t direction, uint8_t height);

/**
 * @brief Imprime arreglo de caracteres en una posición dada del display.
 *
 * @param buff El arreglo de caracteres.
 * @param size El tamaño del buffer.
 * @param x    Posición horizontal de la esquina superior izquierda de la primera letra (desde la izquierda).
 * @param y    Posición vertical de la esquina superior izquierda de la primera letra (desde la izquierda).
 *
 * @pre y + altura de las letras <= 128.
 * @pre x + 4*size - 1 <= 32.
 * @pre Sea b un byte de buff, entonces: 48 <= b <= 57 || 65 <= b <= 90 (valores ASCII válidos).
 */
void render_chars(uint8_t* buff, uint8_t size, uint8_t x, uint8_t y);

/**
 * @brief Pone contador en 0.
 *
 * @param counter El contador a modificar.
 */
void reset_counter(gui_counter_t* counter);

/** TODO: El nombre es porque tiene conflicto con el de timer.h.
 * @brief Incrementa contador según el valor pasado por parámetro.
 *
 * @param counter El contador a modificar.
 * @param value   El valor a sumar.
 *
 * @warning Hay overflow, pasa a cero.
 */
void gui_increment_counter(gui_counter_t* counter, uint8_t value);

#endif /* GUI_H */
