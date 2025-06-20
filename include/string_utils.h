/**
 * @file string_utils.h
 *
 * @brief Módulo para conversión de enteros a arreglos de caracteres y viceversa.
 */
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdint.h>

/**
 * @brief Convertir entero a un arreglo de caracteres.
 *
 * @param value        El entero a convertir.
 * @param str_buff     Puntero al arreglo donde guardar el resultado.
 * @param digit_amount Cantidad de dígitos del número.
 * @pre El tamaño del arreglo debe ser mayor o igual a @c digit_amount .
 * @note Se utiliza la operación de módulo (%).
 */
void alfobi_itoa(uint16_t value, uint8_t* str_buff, uint8_t digit_amount);

/**
 * @brief Convertir string de digitos a número entero sin signo (ASCII to integer).
 *
 * @param str          El arreglo de caracteres que contiene los dígitos.
 * @param digit_amount Cantidad de dígitos del número.
 * @returns El entero de 16 bits correspondiente.
 * @pre @c str solo contiene caracteres entre '0' y '9'.
 */
uint16_t alfobi_atoi(uint8_t* str, uint8_t digit_amount);

#endif /* STRING_UTILS_H */
