#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <game_data.h>

struct arrow {
    uint8_t height;
    arrow_direction_t direction;
};

typedef struct {
    uint8_t start : 4;
    uint8_t end   : 4;
} template_range_t;

typedef struct {
    const uint8_t digit_amount;
    uint8_t* digits;  /* Cada dígito en una posición del array (MSF). Más eficiente para escribir en el display. */
} gui_counter_t;

/**
 * @brief Inicializar.
 */
void init_gui(void);

/**
 * @brief Renderizar y enviar flecha al display.
 *
 * @param arrow_data  Los datos globales de la flecha a dibujar.
 * @param height      La altura actual de la flecha (entre 17 y 127 para derecha/izquiera y entre 20 y 127 para arriba/abajo).
 * @param is_outline  Determina si dibujar el contorno o la flecha.
 *
 * @pre Los parámetros cumplen: page <= 3 && column + start <= 127.
 */
void render_arrow(global_arrow_data_t* arrow_data, uint8_t height, uint8_t is_outline);

/**
 * @brief Limpiar posición donde había flecha en el display.
 *
 * @param arrow_data  Los datos globales de la flecha a dibujar.
 * @param height      La altura actual de la flecha (entre 17 y 127 para derecha/izquiera y entre 20 y 127 para arriba/abajo).
 */
void clean_arrow(global_arrow_data_t* arrow_data, uint8_t height);

/**
 * @brief Imprime arreglo de caracteres en una posición dada del display.
 *
 * @param buff El arreglo de caracteres.
 * @param size El tamaño del buffer.
 * @param x    Posición horizontal de la esquina superior izquierda de la primera letra (desde la izquierda).
 * @param y    Posición vertical de la esquina superior izquierda de la primera letra (desde la izquierda).
 *
 * @pre buff solo contiene caracteres ASCII entre ! y Z. No permite letras minúsculas.
 * @pre y + altura de las letras <= 128.
 * @pre x + 4*size - 1 <= 32.
 * @pre Sea b un byte de buff, entonces: 48 <= b <= 57 || 65 <= b <= 90 (valores ASCII válidos).
 */
void render_chars(uint8_t* buff, uint8_t size, uint8_t x, uint8_t y);

#endif /* GUI_H */
