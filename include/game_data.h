#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdint.h>

/* Juego */
#define INIT_LIVES                       5
#define VERTICAL_ARROW_SPACING           8 /* Espacio mínimo en pixeles entre dos flechas (de todas las columnas). */
#define MAX_ARROW_COUNT_PER_COLUMN       6
#define PROBABILITY_ARRAY_SIZE           16
#define MAX_LEVEL                        8
#define SCORE_ARRAY_LENGTH               9 /* Si se aumenta por encima de 9, hace falta cambiar la lógica. */

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
extern const uint8_t numbers_5x3[58][5];

/* Arreglos probabilísticos de selección de estados, por nivel. */
extern const sequence_mode_t state_probability_array[MAX_LEVEL][PROBABILITY_ARRAY_SIZE];

/* Pasos de secuencia para pasar de nivel. */
extern const int16_t sequence_iterations_per_level[MAX_LEVEL];

/* Velocidades por nivel */
/* Es la cantidad de veces que se debe llamar lower_arrows para que se bajen las flechas. */
extern const uint8_t speed_per_level[MAX_LEVEL];

typedef struct {
    uint16_t array[SCORE_ARRAY_LENGTH];
    uint16_t tail;
} scores_t;

typedef enum {
    RIGHT = 0,
    DOWN  = 1,
    UP    = 2,
    LEFT  = 3,
} arrow_direction_t;

typedef struct {
    uint8_t height; 
    uint8_t active;
} arrow_t;

typedef struct {
    const arrow_direction_t arrow_direction;
    const uint8_t template_size;
    const uint8_t outline_height;
    const uint8_t starting_arrow_height;
    arrow_t arrows[MAX_ARROW_COUNT_PER_COLUMN];
} global_arrow_data_t;

typedef struct {
    const uint8_t* array;
    uint8_t size;
} sized_array_t;

/**
 * @brief Obtener template del contorno la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Estructura del tipo sized_array_t
 */
sized_array_t get_template_outline(arrow_direction_t direction);

/**
 * @brief Obtener template de la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Estructura del tipo sized_array_t.
 */
sized_array_t get_template(arrow_direction_t direction);

#endif /* GAME_DATA_H */
