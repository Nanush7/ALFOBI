#include "string_utils.h"

void alfobi_itoa(uint16_t value, uint8_t* str_buff, uint8_t digit_amount) {
    /* Usamos módulo ya que esta función no se ejecuta en contextos críticos en el tiempo. */
    uint8_t i = digit_amount;
    do {
        i--;
        str_buff[i] = '0' + (value % 10);
    } while (value /= 10);

    if (!i)
        return;

    for (; i; i--) {
        str_buff[i-1] = '0';
    }
}

uint16_t alfobi_atoi(uint8_t* str, uint8_t digit_amount) {
    uint16_t res = 0;

    for (uint8_t i = 0; i < digit_amount; i++) {
        res *= 10;
        res += str[i] - '0';
    }

    return res;
}
