/**
 * @file flash.h
 * @brief Módulo para leer y escribir memoria FLASH.
 */
#ifndef FLASH_H
#define FLASH_H
#include <game_data.h>

/**
 * @brief Inicializar la flash.
 */
void init_flash();

/**
 * @brief Obtener los puntajes guardados en flash.
 *
 * @param res Puntero a memoria donde guardar el resultado.
 */
void get_flash_scores(scores_t* res);

/**
 * @brief Guardar scores en flash.
 *
 * @param scores Puntero a los scores.
 * @note Se desactivan las interrupciones. El proceso de escritura es lento.
 */
void store_scores_to_flash(scores_t* scores);

#endif /* FLASH_H */
