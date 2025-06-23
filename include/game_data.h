/**
 * @addtogroup Game
 * Módulo para manejo de configuración y estado del juego.
 * @{
 * @file game_data.h
 */
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdint.h>

/* Juego */
#define INIT_LIVES                       5
#define VERTICAL_ARROW_SPACING           8 /**< Espacio vertical mínimo (en pixeles) entre dos flechas (de todas las columnas). */
#define MAX_ARROW_COUNT_PER_COLUMN       6
#define PROBABILITY_ARRAY_SIZE           16 /**< Debe ser una potencia de 2. */
#define MAX_LEVEL                        8
#define SCORE_ARRAY_LENGTH               9 /**< Si se aumenta por encima de 9, es necesario cambiar la lógica. */
#define PRESS_TOLERANCE                  4

/* GUI */
#define LEFT_OUTLINE_HEIGHT              120
#define RIGHT_OUTLINE_HEIGHT             120
#define UP_OUTLINE_HEIGHT                121
#define DOWN_OUTLINE_HEIGHT              122
#define STARTING_ARROW_HEIGHT_UP_DOWN    20
#define STARTING_ARROW_HEIGHT_LEFT_RIGHT 17
#define MAX_SCORE_FOR_ARROW              10

#define LEFT_RIGHT_ARROW_SIZE            7
#define UP_DOWN_ARROW_SIZE               4

/** Posibles modos de generación de secuencias. */
typedef enum {
    NONE,
    SINGLE,
    DOUBLE,
    TRIPLE,
    QUAD
} sequence_mode_t;

extern const uint8_t template_left[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_right[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_up[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_down[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_left_outline[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_right_outline[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_up_outline[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_down_outline[UP_DOWN_ARROW_SIZE];

/** Fuente 5x3 de caracteres (entre '!' y 'Z') alineados a la derecha en el byte. */
extern const uint8_t numbers_5x3[58][5];

/** Arreglos probabilísticos de selección de estados, por nivel. */
extern const sequence_mode_t state_probability_array[MAX_LEVEL][PROBABILITY_ARRAY_SIZE];

/** Cantidad de flechas generadas por nivel para pasar al siguiente nivel. */
extern const int16_t sequence_iterations_per_level[MAX_LEVEL];

/** Velocidades por nivel. Es la cantidad de veces que se debe llamar @c lower_arrows para que se bajen las flechas. */
extern const uint8_t speed_per_level[MAX_LEVEL];

/** Representación de scoreboard como arreglo con tope. */
typedef struct {
    /** Si bien los valores se usan como caracteres, tienen que estar alineados para ser almacenados en flash. */
    uint16_t array[SCORE_ARRAY_LENGTH][4];
    /** Tope de @c array . */
    uint16_t tail;
} scores_t;

/** Posibles direcciones de las flechas. */
typedef enum {
    RIGHT = 0,
    DOWN  = 1,
    UP    = 2,
    LEFT  = 3,
} arrow_direction_t;

/** Flecha específica. */
typedef struct {
    uint8_t height; /**< Altura actual de la flecha. */
    uint8_t active; /**< 1 si debe ser tenida en cuenta por la lógica o 0 en caso contrario. */
} arrow_t;

/** Datos globales para cada tipo de flecha. Contiene el arreglo de flechas de la columna correspondiente. */
typedef struct {
    const arrow_direction_t arrow_direction;
    const uint8_t template_size;
    const uint8_t outline_height;
    const uint8_t starting_arrow_height;
    arrow_t arrows[MAX_ARROW_COUNT_PER_COLUMN];
} global_arrow_data_t;

/** Arreglo con tope genérico. */
typedef struct {
    const uint8_t* array;
    uint8_t size;
} sized_array_t;

/**
 * @brief Obtener template del contorno de la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Arreglo con tope que contiene los bytes del contorno.
 */
sized_array_t get_template_outline(arrow_direction_t direction);

/**
 * @brief Obtener template de la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Arreglo con tope que contiene los bytes de la flecha.
 */
sized_array_t get_template(arrow_direction_t direction);

#endif /* GAME_DATA_H */

/** @} */
