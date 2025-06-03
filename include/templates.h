#ifndef TEMPLATES_H
#define TEMPLATES_H

#define LEFT_RIGHT_ARROW_SIZE 7
#define UP_DOWN_ARROW_SIZE    4

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
const uint8_t numbers_5x3[10][5] = {
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
};

#endif /* TEMPLATES_H */
