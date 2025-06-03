/**
 * @file timer_hw.h
 * @brief Definiciones y declaraciones para el módulo de timer por hardware.
 *
 * Este archivo contiene las declaraciones de las funciones utilizadas para
 * configurar e inicializar el timer por hardware.
 */

#ifndef TIMER_HW_H
#define TIMER_HW_H

#include <stdint.h>

/**
 * @brief Configura e inicializa el timer.
 */
void init_timer_hw();

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

void add_timer(timer_t timer);

#endif /* TIMER_HW_H */
