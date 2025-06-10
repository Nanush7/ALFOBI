#include <game.h>
#include <random.h>
#include <assert_test.h>

#define STARTING_ARROW_HEIGHT_UP_DOWN 20
#define STARTING_ARROW_HEIGHT_LEFT_RIGHT 17
#define LEFT_OUTLINE_HIGHT 120
#define UP_OUTLINE_HIGHT 121
#define DOWN_OUTLINE_HIGHT 122
#define RIGHT_OUTLINE_HIGHT 120
#define PROBABILITY_ARRAY_SIZE 10
#define MAX_LEVEL 3
/*
 * Se decrementan los ticks cada 50ms (MS_BETWEEN_TIMER_INTERRUPTS).
 * Así que nos queda que bajan cada 500ms.
 */
#define INIT_SPEED 10

/*=========================*/
/* Estado global del juego */
/*=========================*/

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

/* Juntamos todos los datos en un array para simplificar algoritmos. */
global_arrow_data_t* all_global_arrow_data[4] = {&right_arrow_data, &down_arrow_data, &up_arrow_data, &left_arrow_data};

/* Contadores. */
uint8_t score_counter_array[4];
uint8_t level_counter_array[1];
gui_counter_t score = {4, score_counter_array};
gui_counter_t level = {1, level_counter_array};

/* Modo actual de generación de secuencia. */
sequence_mode_t current_sequence_mode = NONE;

/* Arreglos probabilísticos de selección de estados, por nivel. */
const sequence_mode_t state_probability_array[MAX_LEVEL][PROBABILITY_ARRAY_SIZE] = {
    {NONE, SINGLE, SINGLE, NONE, NONE, SINGLE, NONE, SINGLE, SINGLE, NONE},
    {SINGLE, SINGLE, SINGLE, NONE, NONE, SINGLE, SINGLE, SINGLE, SINGLE, NONE},
    {SINGLE, SINGLE, SINGLE, SINGLE, SINGLE, SINGLE, NONE, SINGLE, SINGLE, SINGLE},
};

/* Pasos de secuencia para pasar de nivel. */
const uint8_t sequence_iterations_per_level[MAX_LEVEL] = {5, 10, 15};
/* Pasos de secuncia actual. */
uint8_t current_sequence_iteration = 0;


/**
 * @brief Obtener número de nivel almacenado en el contador de niveles.
 * Convertido de caradcter a entero sin signo.
 *
 * @returns El número de nivel actual, como entero.
 */
uint8_t get_current_level() {
    return level.digits[0] - '0';
}

/**
 * @brief Obtener struct de datos globales de la flecha según su dirección.
 *
 * @param direction La dirección de la flecha.
 * @return Un puntero al struct de datos correspondiente.
 */
global_arrow_data_t* get_arrow_data(arrow_direction_t direction) {

    global_arrow_data_t* res;
    switch (direction) {
        case UP:
            res = &up_arrow_data;
            break;
        case DOWN:
            res = &down_arrow_data;
            break;
        case LEFT:
            res = &left_arrow_data;
            break;
        case RIGHT:
            res = &right_arrow_data;
            break;
        default:
            ASSERT(0);
            break;
    }

    return res;
}

/**
 * @brief Dada una columna, baja todas las flechas una posición.
 *
 * @param column Los datos de la columna correspondiente.
 */
void lower_column_arrows(global_arrow_data_t* column) {
    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* arrow_ptr = &column->arrows[i];

        if (!arrow_ptr->active)
            continue;

        clean_arrow(column, arrow_ptr->height);
        arrow_ptr->height++;
        if (arrow_ptr->height > 127) {
            arrow_ptr->active = 0;
        } else {
            render_arrow(column, arrow_ptr->height, 0);
        }
    }
}

void lower_arrows(void) {
    for (uint8_t i = 0; i < 4; i++) {
        global_arrow_data_t* arrow_data = all_global_arrow_data[i];

        if (!arrow_data->ticks_lower_arrows) {
            arrow_data->ticks_lower_arrows = arrow_data->speed;
            lower_column_arrows(arrow_data);
        } else {
            arrow_data->ticks_lower_arrows--;
        }

        if (arrow_data->counter_next_arrow) {
            arrow_data->counter_next_arrow--;
        }
    }
}

/**
 * @brief Nuevo tick del juego.
 * Baja flechas y actualiza juego (secuencia, dificultad, etc.)
 */
void game_tick(void) {
    lower_arrows();
    next_sequence();
}

/**
 * @brief Agregar nueva flecha al juego.
 * 
 * @param arrow_data Los datos globales de la flecha a agregar.
 * 
 * @pre Hay un lugar disponible en la columna correspondiente.
 */
void add_new_arrow(global_arrow_data_t* arrow_data) {
    /*
    arrow_t* available_slot = 0;
    for (uint8_t i = MAX_ARROW_COUNT_PER_COLUMN - 1; i; i--) {
        arrow_t* arrow = &(arrow_data->arrows[i]);
        if (arrow->active) {
            available_slot = arrow;
            break;
        }
    }

    // Si no hay lugar para la flecha, lamentablemente cometeremos el delito de droppearla!
    if (!available_slot) {
        return;
    }

    available_slot->direction = arrow_data->arrow_direction;
    available_slot->height = arrow_data->starting_arrow_height;
    available_slot->active = 1; */

    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* arrow = &arrow_data->arrows[i];

        if (!arrow->active) {
            arrow->height = arrow_data->starting_arrow_height;
            arrow->active = 1;
            break;
        }
    }

    /* La proxima flecha puede aparecer después de que la anterior bajó lo suficiente. */
    switch (arrow_data->arrow_direction) {
        case UP:
        case DOWN:
            arrow_data->counter_next_arrow = UP_DOWN_ARROW_SIZE + 1;
            break;
        case LEFT:
        case RIGHT:
            arrow_data->counter_next_arrow = LEFT_RIGHT_ARROW_SIZE + 1;
            break;
        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief Obtener arreglo de columnas disponibles para agregar una nueva flecha.
 * 
 * @param res Puntero al arreglo.
 * @returns Tope del arreglo.
 */
uint8_t get_available_columns(arrow_direction_t* res) {
    uint8_t tope = 0;
    for (uint8_t column_index = 0; column_index < 4; column_index++) {
        const global_arrow_data_t* arrow_data = all_global_arrow_data[column_index];

        if (arrow_data->counter_next_arrow)
            continue;

        for (uint8_t arrow_index = 0; arrow_index < MAX_ARROW_COUNT_PER_COLUMN; arrow_index++) {

            if (!arrow_data->arrows[arrow_index].active) {
                res[tope] = arrow_data->arrow_direction;
                tope++;
                break;
            }
        }
    }

    return tope;
}

/**
 * @brief Generar siguiente paso de la secuencia con una sola flecha.
 * 
 * @returns Los datos globales de la flecha generada o NULL si no hay columnas disponibles.
 */
global_arrow_data_t* generate_sequence_single() {
    /** TODO: Mejorar selección. Usar rand_in_range. */

    arrow_direction_t available_columns[4];
    uint8_t tope = get_available_columns(available_columns);

    if (!tope)
        return 0;

    uint8_t rand_int = rand();
    while ((rand_int & 0x03) >= tope) {
        rand_int <<= 1;
    }

    return get_arrow_data(available_columns[rand_int & 0x03]);
}

/**
 * @brief Sortea y setea el siguiente estado del juego según el nivel.
 */
void next_game_state(void) {
    uint8_t rand_int = rand() & 0x0F;
    if (rand_int >= PROBABILITY_ARRAY_SIZE)
        rand_int -= PROBABILITY_ARRAY_SIZE;

    current_sequence_mode = state_probability_array[get_current_level()-1][rand_int];
}

void next_sequence(void) {
    /* Avanzamos número de secuencia y manejamos nivel de dificultad. */
    uint8_t level_index = get_current_level() - 1;

    if (!current_sequence_iteration && level_index < MAX_LEVEL) {

        increment_counter(&level, 1);
        render_chars(level_counter_array, 1, 0, 6);
        current_sequence_iteration = sequence_iterations_per_level[++level_index];

    } else if (!current_sequence_iteration) {
        /** TODO: Fin del juego. */
        ASSERT(0);
    }

    global_arrow_data_t* next_arrow = 0;
    switch(current_sequence_mode) {
        case SINGLE:
            next_arrow = generate_sequence_single();
            break;
        default:  /* NONE. */
            break;
    }

    if (next_arrow) {
        --current_sequence_iteration;
        add_new_arrow(next_arrow);
    }

    if (next_arrow || current_sequence_mode == NONE) {
        next_game_state(); /** TODO: No queremos hacer esto todas las veces. ¿Ponemos un contador? */
    }
}

/**
 * @brief Inicializar los datos de la columna.
 * 
 * @param column_data Puntero a los datos de la columna.
 */
void init_column(global_arrow_data_t* column_data) {
    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* arrow = &column_data->arrows[i];
        arrow->active    = 0;
        arrow->height    = column_data->starting_arrow_height;
        arrow->direction = column_data->arrow_direction;
    }

    column_data->speed = INIT_SPEED;
    column_data->ticks_lower_arrows = 0;
    column_data->counter_next_arrow   = 0;

    switch (column_data->arrow_direction) {
        case RIGHT:
            column_data->page = 0;
            column_data->outline_height = RIGHT_OUTLINE_HIGHT;
            column_data->starting_arrow_height = STARTING_ARROW_HEIGHT_LEFT_RIGHT;
            break;
        case DOWN:
            column_data->page = 1;
            column_data->outline_height = DOWN_OUTLINE_HIGHT;
            column_data->starting_arrow_height = STARTING_ARROW_HEIGHT_UP_DOWN;
            break;
        case UP:
            column_data->page = 2;
            column_data->outline_height = UP_OUTLINE_HIGHT;
            column_data->starting_arrow_height = STARTING_ARROW_HEIGHT_UP_DOWN;
            break;
        case LEFT:
            column_data->page = 3;
            column_data->outline_height = LEFT_OUTLINE_HIGHT;
            column_data->starting_arrow_height = STARTING_ARROW_HEIGHT_LEFT_RIGHT;
            break;
        default:
            ASSERT(0);
            break;
    }
}

void reset_counter(gui_counter_t* counter) {
    
    for (uint8_t i = 0; i < counter->digit_amount; i++) {
        counter->digits[i] = '0';
    }
}

void increment_counter(gui_counter_t* counter, uint8_t value) {
    
    for (uint8_t increment = 0; increment < value; increment++) {
        
        uint8_t carry = 1;
        for (uint8_t i = counter->digit_amount; i; i--) {
            uint8_t digit_index = i - 1;
            
            if (carry) {
                counter->digits[digit_index]++;
                carry = 0;
            }
            
            if (counter->digits[digit_index] > '9') {
                counter->digits[digit_index] = '0';
                carry = 1;
            }
            
            if (!carry)
            break;
        }
    }
    
    /* Overflow. */
    if (counter->digits[0] > '9')
        reset_counter(counter);
}

void init_game(void) {
    init_column(&left_arrow_data);
    init_column(&right_arrow_data);
    init_column(&up_arrow_data);
    init_column(&down_arrow_data);

    reset_counter(&score);
    reset_counter(&level);
    increment_counter(&level, 1); /* Empieza en nivel 1. */
    render_chars(score_counter_array, 4, 0, 0);
    render_chars(level_counter_array, 1, 0, 6);

    for (uint8_t i = 0; i < 4; i++) {
        render_arrow(all_global_arrow_data[i], all_global_arrow_data[i]->outline_height, 1);
    }

    current_sequence_mode = NONE;
    current_sequence_iteration = sequence_iterations_per_level[0];

    timer_t timer_lower_arrows;
    init_timer(&timer_lower_arrows, 4, game_tick);
    add_timer(timer_lower_arrows);
}
