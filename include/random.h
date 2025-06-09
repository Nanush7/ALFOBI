#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

/**
 * @brief Inicializar módulo de generación de números (pseudo)aleatorios.
 * 
 * @param byte_pointer Puntero a byte utilizado para almacenar número.
 */
void init_random(uint8_t* byte_pointer);

/**
 * @brief Obtener byte pseudoaleatorio.
 * 
 * @returns El byte pseudoaleatorio.
 */
uint8_t rand(void);

/**
 * @brief Obtener un número aleatorio en determinado rango.
 * 
 * @param min Mínimo del rango (incluído).
 * @param max Máximo del rango (incluído).
 * 
 * @returns El número aleatorio generado, perteneciente al rango.
 * 
 * @pre max <= 255.
 * @pre min <= max.
 */
uint8_t rand_in_range(uint8_t min, uint8_t max);

#endif /* RANDOM_H */
