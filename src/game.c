#include <game.h>
#include <game_data.h>
#include <keyboard.h>
#include <assert_test.h>
#include <stdlib.h>
#include <string_utils.h>
#include <flash.h>

/*=========================*/
/* Estado global del juego */
/*=========================*/

/* Datos globales de flechas. */
global_arrow_data_t left_arrow_data = {
    RIGHT, LEFT_RIGHT_ARROW_SIZE, RIGHT_OUTLINE_HEIGHT, STARTING_ARROW_HEIGHT_LEFT_RIGHT, {}
};

global_arrow_data_t down_arrow_data = {
    DOWN, UP_DOWN_ARROW_SIZE, DOWN_OUTLINE_HEIGHT, STARTING_ARROW_HEIGHT_UP_DOWN, {}
};

global_arrow_data_t up_arrow_data = {
    UP, UP_DOWN_ARROW_SIZE, UP_OUTLINE_HEIGHT, STARTING_ARROW_HEIGHT_UP_DOWN, {}
};

global_arrow_data_t right_arrow_data = {
    LEFT, LEFT_RIGHT_ARROW_SIZE, LEFT_OUTLINE_HEIGHT, STARTING_ARROW_HEIGHT_LEFT_RIGHT, {}
};

/* Juntamos todos los datos en un array para simplificar algoritmos. */
global_arrow_data_t* all_global_arrow_data[4] = {&right_arrow_data, &down_arrow_data, &up_arrow_data, &left_arrow_data};

/* Contadores. */
uint8_t score_counter_array[4];
uint8_t level_counter_array[1];
uint8_t lives_counter_array[1];
gui_counter_t score         = {4, score_counter_array};
gui_counter_t level         = {1, level_counter_array};
gui_counter_t lives_counter = {1, lives_counter_array};

/* Pantalla actual */
screen_t current_screen = MAIN;

/* Estado pausa. */
uint8_t paused = 0;
uint8_t debug  = 0;

/* Modo actual de generación de secuencia. */
sequence_mode_t current_sequence_mode = NONE;

/* Pasos de secuencia actual. */
int16_t current_sequence_iteration = 0;

/* Contador para saber cuándo se pueden introducir nuevas flechas. */
uint8_t ticks_next_arrow = 0;

/* Velocidad global */
uint8_t speed = 4;

/* Contador para saber si ya se pueden bajar las flechas. */
uint8_t ticks_lower_arrows = 4;

/* Vidas */
uint8_t lives = INIT_LIVES;

/* Flag para dejar de mandar flechas al final del último nivel. */
uint8_t last_level_stop = 0;

/* Scoreboard. */
scores_t scores;

/* Flag que indica si en el tick actual ya se perdieron vidas. */
uint8_t lost_lives = 0;

/*=============================*/
/* Fin estado global del juego */
/*=============================*/

/**
 * @brief Obtener número de nivel almacenado en el contador de niveles.
 * Convertido de caracter a entero sin signo.
 * Se usa en lugar de atoi porque no usa multiplicaciones.
 *
 * @returns El número de nivel actual, como entero.
 * @pre level es un contador de un solo dígito.
 */
uint8_t get_current_level(void) {
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

void main_menu(void) {
    current_screen = MAIN;
    clean_range(0, 3, 0, 127);

    render_chars("ALFOBI", 6, 4, 60);
    render_chars(":)", 2, 11, 68);
    render_chars("PRESS #", 7, 2, 76);

    for (uint8_t i = 0; i < 4; i++) {
        render_arrow(all_global_arrow_data[i], 85, 0);
    }
}

/**
 * @brief Mostrar pantalla de información.
 */
void show_about() {
    current_screen = ABOUT;
    clean_range(0, 3, 0, 127);
    /** TODO: Terminar. */
    // render_chars("ABOUT", 5, );
}

/**
 * @brief Agregar puntaje actual al scoreboard.
 * Se inserta ordenado y se reemplaza el peor resultado si no hay espacio.
 */
void add_to_scoreboard(uint16_t score_to_add) {

    /* Si la tabla está llena y el score es peor que el último, no se agrega. */
    if (scores.tail == SCORE_ARRAY_LENGTH && scores.array[scores.tail - 1] > score_to_add)
        return;

    /* Si hay lugar para una entrada nueva, la usamos. Si no, sobrescribimos el último. */
    /* Tail es la posición de la nueva última flecha. */
    uint8_t tail = scores.tail < SCORE_ARRAY_LENGTH ? scores.tail : scores.tail - 1;

    scores.array[tail] = score_to_add;

    for (uint8_t i = tail; i; i--) {
        if (scores.array[i] <= scores.array[i-1])
            break;

        uint16_t aux = scores.array[i];
        scores.array[i]    = scores.array[i-1];
        scores.array[i-1]  = aux;
    }

    if (scores.tail < SCORE_ARRAY_LENGTH) {
        scores.tail++;
    }
}

/**
 * @brief Mostrar pantalla de fin del juego.
 *
 * @param win Si pasó el último nivel debe valer 1. 0 en caso contrario.
 */
void game_over(uint8_t win) {
    current_screen = GAME_OVER;
    clean_range(0, 3, 58, 80);
    render_chars("FIN DEL", 7, 3, 61);
    render_chars("JUEGO", 5, 6, 67);

    if (win)
        render_chars("GANASTE", 7, 2, 73);
    else
        render_chars("PERDISTE", 8, 0, 73);

    add_to_scoreboard(alfobi_atoi(score.digits, score.digit_amount));
}

/**
 * @brief Mostrar pantalla del scoreboard.
 */
void show_scoreboard(void) {
    current_screen = SCOREBOARD;
    clean_range(0, 3, 0, 127);
    render_chars("TOP", 3, 9, 13);
    render_chars("SCORES", 6, 3, 19);

    if (!scores.tail) {
        render_chars("404", 3, 10, 30);
        render_chars("NOT", 3, 10, 37);
        render_chars("FOUND", 5, 6, 44);
        return;
    }

    uint8_t score_entry_height = 30;
    for (uint8_t i = 0; i < scores.tail; i++) {
        uint8_t score_str[4];
        alfobi_itoa(scores.array[i], score_str, 4);
        uint8_t index_str[2] = {'1' + i , '-'};
        render_chars(index_str, 1, 4, score_entry_height);
        render_chars(score_str, 4, 15, score_entry_height);
        score_entry_height += 10;
    }
}

/**
 * @brief Alternar pausa y mostrar/ocultar cartel de pausa.
 */
void alternate_pause(void) {
    paused = !paused;
    clean_range(0, 3, 58, 68);
    if (paused)
        render_chars("PAUSA", 5, 6, 61);
}

/**
 * @brief Decrementar contador de vidas y actualizar contador en el display.
 */
void decrement_lives(void) {
    if (debug) return;
    decrement_counter(&lives_counter, 1);
    render_chars(lives_counter.digits, 1, 29, 12);
    if (lives_counter.digits[0] == '0') {
        game_over(0);
    }
}

/**
 * @brief Dada una columna, baja todas las flechas una posición.
 *
 * @param column Los datos de la columna correspondiente.
 * @returns Retorna 0 si no bajaron flechas. 1 en otro caso.
 */
uint8_t lower_column_arrows(global_arrow_data_t* column) {
    uint8_t res = 0;

    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        if (current_screen != GAME) {
            return res;
        }

        arrow_t* arrow_ptr = &column->arrows[i];

        if (!arrow_ptr->active)
            continue;

        /* Pudimos bajar una flecha. */
        res = 1;

        clean_arrow(column, arrow_ptr->height, 1);
        arrow_ptr->height++;
        if (arrow_ptr->height > 127) {
            arrow_ptr->active = 0;
            if (!lost_lives) {
                lost_lives = 1;
                decrement_lives();
            }
        } else {
            render_arrow(column, arrow_ptr->height, 0);
        }
    }

    return res;
}

void lower_arrows(void) {
    ticks_lower_arrows--;
    if (ticks_lower_arrows) {
        return;
    }

    uint8_t columns_are_active = 0;

    for (uint8_t i = 0; i < 4; i++) {
        if (current_screen != GAME)
            return;
        columns_are_active |= lower_column_arrows(all_global_arrow_data[i]);
    }

    if (last_level_stop && !columns_are_active) {
        game_over(1);
        return;
    }

    ticks_lower_arrows = speed;

    if (ticks_next_arrow)
        ticks_next_arrow--;
}

/**
 * @brief Manejar la pulsación sobre una columna específica.
 * Calcula puntaje y vidas.
 *
 * @param arrow_data Los datos globales de la columna correspondiente.
 */
void handle_column_keypress(global_arrow_data_t* arrow_data) {
    /* Buscamos la flecha que se encuentra más abajo. */
    arrow_t* lowest_arrow = 0;
    uint8_t arrow_size = get_template(arrow_data->arrow_direction).size;

    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* candidate = &arrow_data->arrows[i];
        /* Si la flecha no está activa o si está por debajo del outline, la pulsación no actuará sobre ella. */
        if (!candidate->active || candidate->height >= arrow_data->outline_height + arrow_size)
            continue;

        if (!lowest_arrow) {
            lowest_arrow = candidate;
            continue;
        }

        /* La flecha con mayor height es la que se encuentra más baja en el display. */
        if (candidate->height > lowest_arrow->height) {
            lowest_arrow = candidate;
        }
    }

    /* Si no se encuentra, quiere decir que no había flechas, por lo que la pulsación fue errónea y pierde una vida. */
    if (!lowest_arrow) {
        decrement_lives();
        return;
    }

    /* Si se encuentra, vemos qué tan abajo estaba la flecha. Si estaba lo suficientemente abajo manejamos puntuación, si no, pierde una vida. */
    if (lowest_arrow->height <= arrow_data->outline_height - arrow_size) {
        decrement_lives();
    } else {
        /* Asumimos que no se desborda. */
        int16_t score_increment = MAX_SCORE_FOR_ARROW - abs((int16_t)lowest_arrow->height - (int16_t)arrow_data->outline_height);
        increment_counter(&score, score_increment);
        render_chars(score.digits, 4, 17, 0);
    }

    if (current_screen == GAME) {
        lowest_arrow->active = 0;
        clean_arrow(arrow_data, lowest_arrow->height, 0);
    }
}

/**
 * @brief Manejar teclas presionadas.
 */
void handle_keys(void) {
    keys_t pressed_keys = get_pressed_keys();

    if (!*(uint16_t*)&pressed_keys)
        return;

    if (pressed_keys.one && current_screen != GAME) {
        store_scores_to_flash(&scores);
        return;
    }

    if (pressed_keys.a) { /* Desde todas las pantallas va al menú principal. */
        main_menu();
        return;
    }

    if (pressed_keys.b) {
        show_scoreboard();
        return;
    }

    if (pressed_keys.c) {
        show_about();
        return;
    }

    if (pressed_keys.hash) { /* Desde todas las pantallas reinicia el juego. */
        init_game();
        return;
    }

    if (current_screen != GAME)
        return;

    if (pressed_keys.aster) { /* Tecla pausa. */
        alternate_pause();
        return;
    }

    if (paused)
        return;

    if (pressed_keys.d) {
        debug = !debug;
    }

    if (pressed_keys.zero) { /* Left arrow. */
        handle_column_keypress(&left_arrow_data);
    }

    if (pressed_keys.five) { /* Up arrow. */
        handle_column_keypress(&up_arrow_data);
    }

    if (pressed_keys.eight) { /* Down arrow. */
        handle_column_keypress(&down_arrow_data);
    }

    if (pressed_keys.two) { /* Right arrow. */
        handle_column_keypress(&right_arrow_data);
    }
}

void game_tick(void) {
    handle_keys();
    if (!paused && current_screen == GAME) {
        lost_lives = 0;
        lower_arrows();
        next_sequence();
    }
}

/**
 * @brief Agregar nueva flecha al juego.
 * 
 * @param arrow_data Los datos globales de la flecha a agregar.
 * 
 * @pre Hay un lugar disponible en la columna correspondiente.
 */
void add_new_arrow(global_arrow_data_t* arrow_data) {

    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* arrow = &arrow_data->arrows[i];

        if (!arrow->active) {
            arrow->height = arrow_data->starting_arrow_height;
            arrow->active = 1;
            break;
        }
    }
}

/**
 * @brief Sortear y setear el siguiente estado del juego según el nivel.
 */
void next_game_state(void) {
    uint8_t rand_int = rand() & 0x0F;
    if (rand_int >= PROBABILITY_ARRAY_SIZE)
        rand_int -= PROBABILITY_ARRAY_SIZE;

    current_sequence_mode = state_probability_array[get_current_level()-1][rand_int];
}

/**
 * @brief Generar siguiente paso de la secuencia con una determinada cantidad de flechas al mismo tiempo en la misma altura. Dibuja las flechas generadas.
 * Usa el modo de generación de secuencias global.
 */
void generate_arrows() {
    uint8_t column_number, column_number2;

    column_number = rand() & 0b11;

    switch (current_sequence_mode) {
    case SINGLE:
        // Mandamos la flecha por la columna seleccionada.
        add_new_arrow(all_global_arrow_data[column_number]);
        current_sequence_iteration -= 1;
        break;
    case DOUBLE:
        // Sorteamos una columna que no sea la ya sorteada.
        column_number2 = rand() & 0b11;
        if (column_number == column_number2) {
            column_number2 = (column_number + 1) & 0b11;
        }
        add_new_arrow(all_global_arrow_data[column_number]);
        add_new_arrow(all_global_arrow_data[column_number2]);
        current_sequence_iteration -= 2;
        break;
    case TRIPLE:
        // Mandamos una flecha por cada columna, excepto por la que se sorteó.
        for (uint8_t column = 0; column < 4; column++) {
            if (column != column_number) {
                add_new_arrow(all_global_arrow_data[column]);
            }
        }
        current_sequence_iteration -= 3;
        break;
    case QUAD:
        for (uint8_t column = 0; column < 4; column++) {
            add_new_arrow(all_global_arrow_data[column]);
        }
        current_sequence_iteration -= 4;
        break;
    case NONE: /* NONE */
        next_game_state();
        return;
    }

    next_game_state();
}

void next_sequence(void) {
    if (ticks_next_arrow || last_level_stop)
        return;

    /* Avanzamos número de secuencia y manejamos nivel de dificultad. */
    uint8_t level_number = get_current_level();

    if (current_sequence_iteration <= 0 && level_number >= MAX_LEVEL) {
        last_level_stop = 1;
        return;
    }

    if (current_sequence_iteration <= 0) {
        current_sequence_iteration = sequence_iterations_per_level[level_number];
        speed = speed_per_level[level_number];
        ticks_lower_arrows = speed;
        increment_counter(&level, 1);
        render_chars(level_counter_array, 1, 29, 6);
    }

    /* La proxima flecha puede aparecer después de que la anterior bajó lo suficiente. */
    /* LEFT_RIGHT_ARROW_SIZE porque es el más grande entre los dos tamaños de flechas. */
    ticks_next_arrow = LEFT_RIGHT_ARROW_SIZE + VERTICAL_ARROW_SPACING;

    generate_arrows();
}

/**
 * @brief Inicializar los datos de la columna.
 * 
 * @param column_data Puntero a los datos de la columna.
 */
void init_column(global_arrow_data_t* column_data) {
    for (uint8_t i = 0; i < MAX_ARROW_COUNT_PER_COLUMN; i++) {
        arrow_t* arrow   = &column_data->arrows[i];
        arrow->active    = 0;
        arrow->height    = column_data->starting_arrow_height;
    }
}

void reset_counter(gui_counter_t* counter) {
    
    for (uint8_t i = 0; i < counter->digit_amount; i++) {
        counter->digits[i] = '0';
    }
}

void increment_counter(gui_counter_t* counter, uint8_t value) {
    
    for (uint8_t increment = 0; increment < value; increment++) {
        
        for (uint8_t i = counter->digit_amount; i; i--) {
            uint8_t digit_index = i - 1;

            counter->digits[digit_index]++;

            if (counter->digits[digit_index] > '9') {
                counter->digits[digit_index] = '0';
            } else {
                break;
            }
        }
    }
    
    /* Overflow. */
    if (counter->digits[0] > '9')
        reset_counter(counter);
}

void decrement_counter(gui_counter_t* counter, uint8_t value) {

    for (uint8_t increment = 0; increment < value; increment++) {

        for (uint8_t i = counter->digit_amount; i; i--) {
            uint8_t digit_index = i - 1;

            counter->digits[digit_index]--;

            if (counter->digits[digit_index] < '0') {
                counter->digits[digit_index] = '9';
            } else {
                break;
            }
        }
    }

    /* Overflow. */
    if (counter->digits[counter->digit_amount - 1] < '0')
        for (uint8_t i = counter->digit_amount; i; i--) {
            counter->digits[i - 1] = '9';
        }
}

void init_game(void) {
    current_screen = GAME;

    init_gui();

    init_column(&left_arrow_data);
    init_column(&right_arrow_data);
    init_column(&up_arrow_data);
    init_column(&down_arrow_data);

    reset_counter(&score);
    level.digits[0] = '1';
    lives_counter.digits[0] = INIT_LIVES + '0'; /* Convertimos el valor de vidas a un valor válido ASCII. */
    render_chars(score_counter_array, score.digit_amount, 17, 0);
    render_chars(level_counter_array, level.digit_amount, 29, 6);
    render_chars(lives_counter_array, lives_counter.digit_amount, 29, 12);
    render_chars("P:", 2, 0, 0);  /* Puntaje. */
    render_chars("N:", 2, 0, 6);  /* Nivel. */
    render_chars("V:", 2, 0, 12); /* Vidas. */

    for (uint8_t i = 0; i < 4; i++) {
        render_arrow(all_global_arrow_data[i], all_global_arrow_data[i]->outline_height, 1);
    }

    paused = 0;
    current_sequence_mode = SINGLE;
    current_sequence_iteration = sequence_iterations_per_level[0];
    speed = speed_per_level[0];
    ticks_next_arrow = 0;
    ticks_lower_arrows = speed;
    lives = INIT_LIVES;
    lost_lives = 0;
    last_level_stop = 0;
}

void set_scores(scores_t* scores_param) {
    scores = *scores_param;
}
