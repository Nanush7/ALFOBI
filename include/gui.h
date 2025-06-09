#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include "templates.h"

/** TODO: Mover a game las estructuras de abajo. */
#define MAX_ARROW_COUNT_PER_COLUMN 10

#define STARTING_ARROW_HEIGHT_UP_DOWN 20
#define STARTING_ARROW_HEIGHT_LEFT_RIGHT 17
#define LEFT_OUTLINE_HIGHT 120
#define UP_OUTLINE_HIGHT 121
#define DOWN_OUTLINE_HIGHT 122
#define RIGHT_OUTLINE_HIGHT 120
#define PROBABILITY_ARRAY_SIZE 10
#define MAX_LEVEL 3
#define INIT_SPEED 10

typedef enum {
    RIGHT = 0,
    DOWN  = 1,
    UP    = 2,
    LEFT  = 3,
} arrow_direction_t;

typedef struct {
    arrow_direction_t direction;
    uint8_t height; 
    uint8_t active; /* A modo de prototipo! */
} arrow_t;

typedef struct {
    const arrow_direction_t arrow_direction;
    const uint8_t template_size;
    arrow_t arrows[MAX_ARROW_COUNT_PER_COLUMN];
    uint8_t ticks_lower_arrows;
    uint8_t counter_next_arrow;
    uint8_t outline_height;
    uint8_t speed;
    uint8_t page;
    uint8_t starting_arrow_height;
} global_arrow_data_t;

struct arrow {
    uint8_t height;
    arrow_direction_t direction;
};

typedef struct {
    uint8_t start : 4;
    uint8_t end   : 4;
} template_range_t;

/* Datos globales de flechas. */
global_arrow_data_t left_arrow_data = {
    RIGHT, LEFT_RIGHT_ARROW_SIZE, {}, 0, 0, RIGHT_OUTLINE_HIGHT, INIT_SPEED, 0, STARTING_ARROW_HEIGHT_LEFT_RIGHT
};

global_arrow_data_t down_arrow_data = {
    DOWN, UP_DOWN_ARROW_SIZE, {}, 0, 0, DOWN_OUTLINE_HIGHT, INIT_SPEED, 1, STARTING_ARROW_HEIGHT_UP_DOWN
};

global_arrow_data_t up_arrow_data = {
    UP, UP_DOWN_ARROW_SIZE, {}, 0, 0, UP_OUTLINE_HIGHT, INIT_SPEED, 2, STARTING_ARROW_HEIGHT_UP_DOWN
};

global_arrow_data_t right_arrow_data = {
    LEFT, LEFT_RIGHT_ARROW_SIZE, {}, 0, 0, LEFT_OUTLINE_HIGHT, INIT_SPEED, 3, STARTING_ARROW_HEIGHT_LEFT_RIGHT
};

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
void render_arrow(const global_arrow_data_t* arrow_data, uint8_t height, uint8_t is_outline);

/**
 * @brief Limpiar posición donde había flecha en el display.
 *
 * @param arrow_data  Los datos globales de la flecha a dibujar.
 * @param height      La altura actual de la flecha (entre 17 y 127 para derecha/izquiera y entre 20 y 127 para arriba/abajo).
 */
void clean_arrow(const global_arrow_data_t* arrow_data, uint8_t height);

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
