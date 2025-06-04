#include <gui.h>
#include <display.h>
#include <assert_test.h>
#include <stdint.h>
#include <templates.h>

#define HORIZONTAL_SEPARATOR_POSITION 22
#define MAX_HEIGHT_VALUE              127
#define MAX_WIDTH_VALUE               31

global_arrow_data_t up_arrow_data = {
    template_up, template_up_outline, UP, UP_DOWN_ARROW_SIZE, 121, 2, 2
};

global_arrow_data_t down_arrow_data = {
    template_down, template_down_outline, DOWN, UP_DOWN_ARROW_SIZE, 122, 2, 1
};

global_arrow_data_t left_arrow_data = {
    template_right, template_right_outline, RIGHT, LEFT_RIGHT_ARROW_SIZE, 120, 2, 0
};

global_arrow_data_t right_arrow_data = {
    template_left, template_left_outline, LEFT, LEFT_RIGHT_ARROW_SIZE, 120, 2, 3
};

/**
 * @brief Combinar bytes de elementos que se solapan en el display.
 * Dado un byte y una posición donde se dibujará,
 * retorna el byte resultante de combinarlo con los elementos estáticos en dicha posición.
 *
 * @param byte       El byte inicial a dibujar.
 * @param arrow_data Datos según el tipo de flecha.
 * @param height     Columna del display donde se dibujará el byte.
 * @return El byte final (combinado) a escribir.
 */
uint8_t combine_with_static_elements(uint8_t byte, const global_arrow_data_t* arrow_data, uint8_t height) {

    uint8_t res            = byte;
    uint8_t outline_height = arrow_data->outline_height;

    if (height >= outline_height && height <= outline_height + arrow_data->template_size - 1)
        res |= arrow_data->outline_template[height - outline_height];

    return res;
}

/**
 * @brief Calcula el rango del template a renderizar.
 * Como las flechas pueden estar por arriba/abajo del área visible,
 * necesitamos saber qué partes de la flecha estarán visibles. Procedimiento privado.
 *
 * @param  arrow_data Datos de la flecha según su dirección.
 * @param  height     La altura actual de la flecha.
 * @return El primer y último índice a renderizar del template de la flecha.
 */
template_range_t get_template_range(const global_arrow_data_t* arrow_data, uint8_t height) {
    /*
     * Las flechas empiezan a aparecer en la altura HORIZONTAL_SEPARATOR_POSITION + 1.
     * y empiezan a desaparecer en la altura 127.
     */

    template_range_t res;

    /** TODO: UP y DOWN aparecen en la altura 20
    *         LEFT y RIGHT aparecen en la altura 17.
    *         Todas las flechas desaparecen después de la altura 127.
    */
    uint8_t template_size = arrow_data->template_size;

    if (height <= HORIZONTAL_SEPARATOR_POSITION) {
        res.start = HORIZONTAL_SEPARATOR_POSITION - height + 1;
        res.end   = template_size - 1;
    } else if (height + template_size > MAX_HEIGHT_VALUE + 1) {
        res.start = 0;
        res.end   = MAX_HEIGHT_VALUE - height;
    } else {
        res.start = 0;
        res.end   = template_size - 1;
    }

    return res;
}

/**
 * @brief Dado un tipo de flecha y una altura, setea el cursor y manda al display los bytes correspondientes.
 * 
 * @param arrow_data Datos según el tipo de flecha.
 * @param height     Columna del display donde comenzar a escribir los bytes.
 * @param is_outline Determina si dibujar el contorno o la flecha.
 * 
 * @pre Los parámetros cumplen: page <= 3 && column + start <= 127.
 */
void render_arrow_template(const global_arrow_data_t* arrow_data, uint8_t height, uint8_t is_outline) {

    template_range_t range = get_template_range(arrow_data, height);
    set_cursor_position(arrow_data->page, height + range.start);

    const uint8_t* template_aux;
    if (is_outline)
        template_aux = arrow_data->outline_template;
    else
        template_aux = arrow_data->arrow_template;

    /* Iniciamos en la posición correspondiente del template seleccionado. */
    template_aux += range.start;

    for (uint8_t i = 0; i <= range.end - range.start; i++) {
        if (is_outline)
            write_data(template_aux[i]);
        else
            write_data(combine_with_static_elements(template_aux[i], arrow_data, height + i));
    }
}

void init_gui(void) {

    init_display();

    /* Limpiamos display */
    set_cursor_position(0, 0);
    for (uint8_t page = 4; page; page--) {
        for (uint8_t column = 128; column; column--) {
            if (column == 106) {
                /* Aprovechamos la pasada para dibujar la barra horizontal que divide el área de juego. */
                write_data(0xFF);
            } else {
                write_data(0x00);
            }
        }
    }
    render_arrow_template(&left_arrow_data, left_arrow_data.outline_height, 1);
    render_arrow_template(&right_arrow_data, right_arrow_data.outline_height, 1);
    render_arrow_template(&up_arrow_data, up_arrow_data.outline_height, 1);
    render_arrow_template(&down_arrow_data, down_arrow_data.outline_height, 1);
}

/**
 * @brief Obtener struct de datos globales de la flecha según su dirección.
 *
 * @param direction La dirección de la flecha.
 * @return Un puntero al struct de datos correspondiente.
 */
global_arrow_data_t* get_arrow_data_by_direction(arrow_direction_t direction) {

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
            break;
    }

    return res;
}

void render_arrow(arrow_direction_t direction, uint8_t height, uint8_t is_outline) {
    const global_arrow_data_t* arrow_data = get_arrow_data_by_direction(direction);
    render_arrow_template(arrow_data, height, is_outline);
}

void clean_arrow(arrow_direction_t direction, uint8_t height) {

    const global_arrow_data_t* arrow_data = get_arrow_data_by_direction(direction);
    template_range_t range = get_template_range(arrow_data, height);

    /* Establecemos las páginas correspondientes a la dirección de la flecha. */
    set_cursor_position(arrow_data->page, height + range.start);

    for (uint8_t i = 0; i <= range.end - range.start; i++) {
        write_data(combine_with_static_elements(0, arrow_data, height + i));
    }
}

void render_chars(uint8_t* buff, uint8_t size, uint8_t x, uint8_t y) {
    ASSERT(y + 5 <= 128 && x + 4*size - 1 <= 32);

    /*
     * Cada caracter puede estar distribuído en una o dos páginas, en cinco columnas.
     * Se calcula, para toda la columna, los bits a prender, luego se hace la separación
     * en páginas y se manda al display. Las precondiciones son importantes.
     */

    /* Columnas de 32 bits. */
    uint32_t columns[5] = {0, 0, 0, 0, 0};

    /* Shift para llegar a x después de escribir todos los caracteres alineados a la derecha. */
    uint8_t required_shift = MAX_WIDTH_VALUE - (4*size - 1) - x + 1;

    /* Para cada columna, calcular para cada caracter, shifteando hacia la izquierda. */

    for (uint8_t character = 0; character < size; character++) {

        const uint8_t* template = numbers_5x3[buff[character] - '0'];  /** TODO: por ahora solo para números. */

        for (uint8_t column = 0; column < 5; column++) {

            columns[column] |= template[column];

            if (character < size - 1)
                columns[column] <<= 4; /* 4 es el ancho de las letras (3) más un espacio. */
            else
                columns[column] <<= required_shift; /* Shifteamos todo hasta alcanzar el x. */
        }
    }

    /**
     * TODO: Podríamos hacer que no se escriban las páginas donde no hay ningún pedazo de caracter,
     * pero no se me ocurrió una forma fácil y capaz que no es necesario (simplemente dedicamos 5 columnas enteras para escribir los chars).
     * Al empezar a escribir, si no hay modificaciones para la página, no es escribimos nada (tampoco 0s).
     * Después de que se encontró alguna página con contenido, la próxima vacía termina el algoritmo.
     */
    for (uint8_t page = 0; page < 4; page++) {

        set_cursor_position(page, y);
        for (uint8_t column = 0; column < 5; column++) {
            if (page)
                columns[column] >>= 8;
            write_data(columns[column] & 0xFF);
        }
    }
}

void reset_counter(gui_counter_t* counter) {

    for (uint8_t i = 0; i < counter->digit_amount; i++) {
        counter->digits[i] = '0';
    }
}

void gui_increment_counter(gui_counter_t* counter, uint8_t value) {

    /** TODO: Medio caquita esto. Si se les ocurre una forma más simple y legible, bienvenida sea! */
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
