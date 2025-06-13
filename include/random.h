#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

/**
 * @brief Inicializar módulo de generación de números (pseudo)aleatorios.
 */
void init_random(uint16_t seed);

/**
 * @brief Generar el siguiente número número pseudoaleatorio.
 */
void next_rand(void);

/**
 * @brief Obtener número pseudoaleatorio.
 *
 * @returns El número pseudoaleatorio.
 */
uint16_t get_rand(void);

#endif /* RANDOM_H */
