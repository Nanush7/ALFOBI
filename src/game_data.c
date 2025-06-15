#include <game_data.h>
#include <assert_test.h>

const uint8_t template_left[LEFT_RIGHT_ARROW_SIZE] = {
    0b00001000,
    0b00011000,
    0b00111000,
    0b01111000,
    0b00111000,
    0b00011000,
    0b00001000
};

const uint8_t template_right[LEFT_RIGHT_ARROW_SIZE] = {
    0b00010000,
    0b00011000,
    0b00011100,
    0b00011110,
    0b00011100,
    0b00011000,
    0b00010000
};

const uint8_t template_up[UP_DOWN_ARROW_SIZE] = {
    0b00010000,
    0b00111000,
    0b01111100,
    0b11111110
};

const uint8_t template_down[UP_DOWN_ARROW_SIZE] = {
    0b01111111,
    0b00111110,
    0b00011100,
    0b00001000
};

const uint8_t template_left_outline[LEFT_RIGHT_ARROW_SIZE] = {
    0b00001000,
    0b00011000,
    0b00101000,
    0b01001000,
    0b00101000,
    0b00011000,
    0b00001000
};

const uint8_t template_right_outline[LEFT_RIGHT_ARROW_SIZE] = {
    0b00010000,
    0b00011000,
    0b00010100,
    0b00010010,
    0b00010100,
    0b00011000,
    0b00010000
};

const uint8_t template_up_outline[UP_DOWN_ARROW_SIZE] = {
    0b00010000,
    0b00101000,
    0b01000100,
    0b11111110
};

const uint8_t template_down_outline[UP_DOWN_ARROW_SIZE] = {
    0b01111111,
    0b00100010,
    0b00010100,
    0b00001000
};

/* Números 5x3 alineados a la derecha en el byte. */
const uint8_t numbers_5x3[58][5] = {
    {0b010, 0b010, 0b010, 0b000, 0b010}, /* ! */
    {0b101, 0b101, 0b000, 0b000, 0b000}, /* " */
    {0b101, 0b111, 0b101, 0b111, 0b101}, /* # */
    {0b111, 0b110, 0b111, 0b011, 0b111}, /* $ */
    {0b100, 0b001, 0b001, 0b100, 0b001}, /* % */
    {0b010, 0b111, 0b100, 0b111, 0b010}, /* & */
    {0b010, 0b010, 0b000, 0b000, 0b000}, /* ' */
    {0b010, 0b100, 0b100, 0b100, 0b010}, /* ( */
    {0b010, 0b001, 0b001, 0b001, 0b010}, /* ) */
    {0b000, 0b010, 0b101, 0b010, 0b000}, /* * */
    {0b000, 0b010, 0b111, 0b010, 0b000}, /* + */
    {0b000, 0b000, 0b000, 0b001, 0b011}, /* , */
    {0b000, 0b000, 0b111, 0b000, 0b000}, /* - */
    {0b000, 0b000, 0b000, 0b000, 0b010}, /* . */
    {0b000, 0b001, 0b010, 0b100, 0b000}, /* / */
    {0b111, 0b101, 0b101, 0b101, 0b111}, /* 0 */
    {0b110, 0b010, 0b010, 0b010, 0b111}, /* 1 */
    {0b111, 0b001, 0b111, 0b100, 0b111}, /* 2 */
    {0b111, 0b001, 0b111, 0b001, 0b111}, /* 3 */
    {0b101, 0b101, 0b111, 0b001, 0b001}, /* 4 */
    {0b111, 0b100, 0b111, 0b001, 0b111}, /* 5 */
    {0b111, 0b100, 0b111, 0b101, 0b111}, /* 6 */
    {0b111, 0b001, 0b001, 0b001, 0b001}, /* 7 */
    {0b111, 0b101, 0b111, 0b101, 0b111}, /* 8 */
    {0b111, 0b101, 0b111, 0b001, 0b111}, /* 9 */
    {0b000, 0b010, 0b000, 0b010, 0b000}, /* : */
    {0b000, 0b010, 0b000, 0b010, 0b110}, /* ; */
    {0b001, 0b010, 0b100, 0b010, 0b001}, /* < */
    {0b000, 0b111, 0b000, 0b111, 0b000}, /* = */
    {0b100, 0b010, 0b001, 0b010, 0b100}, /* > */
    {0b111, 0b001, 0b011, 0b000, 0b010}, /* ? */
    {0b111, 0b101, 0b101, 0b100, 0b111}, /* @ */
    {0b111, 0b101, 0b111, 0b101, 0b101}, /* A */
    {0b111, 0b101, 0b110, 0b101, 0b111}, /* B */
    {0b111, 0b100, 0b100, 0b100, 0b111}, /* C */
    {0b110, 0b101, 0b101, 0b101, 0b110}, /* D */
    {0b111, 0b100, 0b111, 0b100, 0b111}, /* E */
    {0b111, 0b100, 0b111, 0b100, 0b100}, /* F */
    {0b111, 0b100, 0b101, 0b101, 0b111}, /* G */
    {0b101, 0b101, 0b111, 0b101, 0b101}, /* H */
    {0b111, 0b010, 0b010, 0b010, 0b111}, /* I */
    {0b001, 0b001, 0b001, 0b101, 0b111}, /* J */
    {0b101, 0b101, 0b110, 0b101, 0b101}, /* K */
    {0b100, 0b100, 0b100, 0b100, 0b111}, /* L */
    {0b101, 0b111, 0b101, 0b101, 0b101}, /* M */
    {0b111, 0b101, 0b101, 0b101, 0b101}, /* N */
    {0b111, 0b101, 0b101, 0b101, 0b111}, /* O */
    {0b111, 0b101, 0b111, 0b100, 0b100}, /* P */
    {0b111, 0b101, 0b101, 0b111, 0b010}, /* Q */
    {0b111, 0b101, 0b110, 0b101, 0b101}, /* R */
    {0b111, 0b100, 0b111, 0b001, 0b111}, /* S */
    {0b111, 0b010, 0b010, 0b010, 0b010}, /* T */
    {0b101, 0b101, 0b101, 0b101, 0b111}, /* U */
    {0b101, 0b101, 0b101, 0b101, 0b010}, /* V */
    {0b101, 0b101, 0b101, 0b111, 0b101}, /* W */
    {0b101, 0b101, 0b010, 0b101, 0b101}, /* X */
    {0b101, 0b101, 0b111, 0b010, 0b010}, /* Y */
    {0b111, 0b001, 0b010, 0b100, 0b111}, /* Z */
};


/* @brief Obtener template del contorno la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Estructura del tipo sized_array_t
 */
sized_array_t get_template_outline(arrow_direction_t direction) {
    sized_array_t res;

    switch (direction) {
        case LEFT:
            res.size = LEFT_RIGHT_ARROW_SIZE;
            res.array = template_left_outline;
            break;
        case UP:
            res.size = UP_DOWN_ARROW_SIZE;
            res.array = template_up_outline;
            break;
        case DOWN:
            res.size = UP_DOWN_ARROW_SIZE;
            res.array = template_down_outline;
            break;
        case RIGHT:
            res.size = LEFT_RIGHT_ARROW_SIZE;
            res.array = template_right_outline;
            break;
        default:
            ASSERT(0);
            break;
    }

    return res;
}

/**
 * @brief Obtener template de la flecha y su tamaño según su dirección.
 *
 * @param  direction La direccion de la flecha correspondiente.
 * @return Estructura del tipo sized_array_t.
 */
sized_array_t get_template(arrow_direction_t direction) {
    sized_array_t res;

    switch (direction) {
        case LEFT:
            res.size = LEFT_RIGHT_ARROW_SIZE;
            res.array = template_left;
            break;
        case UP:
            res.size = UP_DOWN_ARROW_SIZE;
            res.array = template_up;
            break;
        case DOWN:
            res.size = UP_DOWN_ARROW_SIZE;
            res.array = template_down;
            break;
        case RIGHT:
            res.size = LEFT_RIGHT_ARROW_SIZE;
            res.array = template_right;
            break;
        default:
            ASSERT(0);
            break;
    }

    return res;
}
