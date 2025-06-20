/**
 * @addtogroup gui
 * @{
 * @file gui.c
 */
#include <gui.h>
#include <display.h>
#include <assert_test.h>
#include <stdint.h>

#define HORIZONTAL_SEPARATOR_POSITION 22
#define MAX_HEIGHT_VALUE              127
#define MAX_WIDTH_VALUE               31

/**
 * @brief Combinar bytes de elementos que se solapan en el display.
 * Dado un byte y una posición donde se dibujará,
 * retorna el byte resultante de combinarlo con los elementos estáticos en dicha posición.
 * Usado para mostrar flechas pasando sobre los contornos.
 *
 * @param byte       El byte inicial a dibujar.
 * @param arrow_data Datos según el tipo de flecha.
 * @param height     Columna del display donde se dibujará el byte.
 * @return El byte final (mezcla de @c byte y los elementos estáticos que correspondan según los otros parámetros).
 */
uint8_t combine_with_static_elements(uint8_t byte, global_arrow_data_t* arrow_data, uint8_t height) {

    uint8_t res            = byte;
    uint8_t outline_height = arrow_data->outline_height;

    sized_array_t outline_template = get_template_outline(arrow_data->arrow_direction);
    if (height >= outline_height && height <= outline_height + outline_template.size - 1)
        res |= outline_template.array[(uint8_t)(height - outline_height)];

    return res;
}

/**
 * @brief Calcula el rango del template a renderizar.
 * Como las flechas pueden estar por arriba/abajo del área visible,
 * necesitamos saber qué partes de la flecha estarán visibles.
 * Las flechas empiezan a aparecer en la altura HORIZONTAL_SEPARATOR_POSITION + 1 y empiezan a desaparecer en la altura 127.
 *
 * @param  arrow_data Datos globales de la flecha correspondiente.
 * @param  height     La altura actual de la flecha.
 * @return El primer y último índice a renderizar del template de la flecha.
 */
template_range_t get_template_range(global_arrow_data_t* arrow_data, uint8_t height) {

    template_range_t res;

    sized_array_t arrow_template = get_template(arrow_data->arrow_direction);

    if (height <= HORIZONTAL_SEPARATOR_POSITION) {
        res.start = HORIZONTAL_SEPARATOR_POSITION - height + 1;
        res.end   = arrow_template.size - 1;
    } else if (height + arrow_template.size > MAX_HEIGHT_VALUE + 1) {
        res.start = 0;
        res.end   = MAX_HEIGHT_VALUE - height;
    } else {
        res.start = 0;
        res.end   = arrow_template.size - 1;
    }

    return res;
}

void render_arrow(global_arrow_data_t* arrow_data, uint8_t height, uint8_t is_outline) {

    template_range_t range = get_template_range(arrow_data, height);
    set_cursor_position(arrow_data->arrow_direction, height + range.start);

    const uint8_t* template_aux;
    if (is_outline)
        template_aux = get_template_outline(arrow_data->arrow_direction).array;
    else
        template_aux = get_template(arrow_data->arrow_direction).array;

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
}

void clean_range(uint8_t min_page, uint8_t max_page, uint8_t min_column, uint8_t max_column) {
    ASSERT(min_page <= max_column && min_column <= max_column && max_column < 128 && max_page < 4);

    for (uint8_t page = min_page; page <= max_page; page++) {
        set_cursor_position(page, min_column);

        for (uint8_t column = min_column; column <= max_column; column++) {
            write_data(0x00);
        }
    }
}

void clean_arrow(global_arrow_data_t* arrow_data, uint8_t height, uint8_t fast_clean) {

    template_range_t range = get_template_range(arrow_data, height);

    /* Establecemos las páginas correspondientes a la dirección de la flecha. */
    set_cursor_position(arrow_data->arrow_direction, height + range.start);

    if (fast_clean) {
        write_data(combine_with_static_elements(0, arrow_data, height));
        return;
    }

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

        const uint8_t* template = 0;
        if (buff[character] != ' ')
            template = numbers_5x3[buff[character] - '!'];

        for (uint8_t column = 0; column < 5; column++) {

            if (buff[character] == ' ') {
                columns[column] <<= 4;
                continue;
            }

            columns[column] |= template[column];

            if (character < size - 1)
                columns[column] <<= 4; /* 4 es el ancho de las letras (3) más un espacio. */
            else
                columns[column] <<= required_shift; /* Shifteamos todo hasta alcanzar el x. */
        }
    }

    for (uint8_t page = 0; page < 4; page++) {

        uint8_t empty = 1;
        for (uint8_t column = 0; column < 5; column++) {
            if (columns[column] & 0xFF) {
                empty = 0;
                break;
            }
        }

        set_cursor_position(page, y);
        for (uint8_t column = 0; column < 5; column++) {
            if (!empty)
                write_data(columns[column] & 0xFF);
            columns[column] >>= 8;
        }
    }
}

/** @} */
