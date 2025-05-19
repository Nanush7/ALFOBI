/**
 * @file timer_hw.h
 * @brief Definiciones y declaraciones para el módulo de timer por hardware.
 *
 * Este archivo contiene las declaraciones de las funciones utilizadas para
 * configurar e inicializar el timer por hardware.
 *
 * @version 2.0
 * @date 08/04/2025
 */

#ifndef _TIMER_HW_H_
#define _TIMER_HW_H_

#include <stdint.h>

typedef enum {
    TIMER_SOURCE_CRYSTAL,
    TIMER_SOURCE_VLO
} timer_source_t;

/**
 * @brief Configura e inicializa el timer con la fuente indicada.
 *
 * @param source Fuente seleccionada.
 */
void init_timer(timer_source_t source);

/**
 * @brief Deshabilita las interrupciones del timer de hardware (CCIE0)
 *
 */
void disable_interrupt_timerhw();

/**
* @brief Habilita las interrupciones del timer de hardware (CCIE0)
*
*/
void enable_interrupt_timerhw();

#endif
