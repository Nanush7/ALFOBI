#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdint.h>

/* Juego */
#define INIT_SPEED                 1 /* Es la cantidad de veces que se debe llamar lower_arrows para que se bajen las flechas. */
#define VERTICAL_ARROW_SPACING     7 /* Espacio mínimo en pixeles entre dos flechas (de todas las columnas). */
#define MAX_ARROW_COUNT_PER_COLUMN 6
#define PROBABILITY_ARRAY_SIZE     10
#define MAX_LEVEL                  3

/* GUI */
#define LEFT_OUTLINE_HEIGHT              120
#define RIGHT_OUTLINE_HEIGHT             120
#define UP_OUTLINE_HEIGHT                121
#define DOWN_OUTLINE_HEIGHT              122
#define STARTING_ARROW_HEIGHT_UP_DOWN    20
#define STARTING_ARROW_HEIGHT_LEFT_RIGHT 17

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

#endif /* GAME_DATA_H */
