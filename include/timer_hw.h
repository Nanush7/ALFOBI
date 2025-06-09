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
#include <timer.h>
#include <func_queue.h>

/**
 * @brief Configura e inicializa el timer.
 */
void init_timer_hw(void);

/**
 * @brief Deshabilita las interrupciones del timer de hardware (CCIE0)
 */
void disable_interrupt_timerhw(void);

/**
* @brief Habilita las interrupciones del timer de hardware (CCIE0)
*
*/
void enable_interrupt_timerhw(void);

/**
 * @brief Establece qué función se invocará cuando se alcance el target del TimerA1.
 * 
 * @param callback Puntero a la función correspondiente.
 */
void set_timer_A1_callback(func* callback);

/**
 * @brief Habilita TimerA1.
 * 
 * @note TimerA1 se deshabilita cuando interrumpe.
 */
void enable_timer_A1(void);

#endif /* TIMER_HW_H */
