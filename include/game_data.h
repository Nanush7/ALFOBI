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

extern const uint8_t template_left[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_right[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_up[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_down[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_left_outline[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_right_outline[LEFT_RIGHT_ARROW_SIZE];
extern const uint8_t template_up_outline[UP_DOWN_ARROW_SIZE];
extern const uint8_t template_down_outline[UP_DOWN_ARROW_SIZE];
extern const uint8_t numbers_5x3[58][5];

typedef enum {
    RIGHT = 0,
    DOWN  = 1,
    UP    = 2,
    LEFT  = 3,
} arrow_direction_t;

typedef struct {
    arrow_direction_t direction; /** TODO: Esto puede no ser necesario ya que guardamos todas las flechas con misma dirección en un mismo arreglo. */
    uint8_t height; 
    uint8_t active;
} arrow_t;

typedef struct {
    const arrow_direction_t arrow_direction;
    const uint8_t template_size; /** Este se puede deducir a partir de la dirección */
    arrow_t arrows[MAX_ARROW_COUNT_PER_COLUMN];
    uint8_t outline_height; /** TODO: en caso de no modificar esto en tiempo de ejecución, sacar este campo. */
    uint8_t page; /** TODO: en caso de no modificar esto en tiempo de ejecución, sacar este campo. */
    uint8_t starting_arrow_height; /** Este se puede deducir a partir de la dirección */
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
