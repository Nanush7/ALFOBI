/**
 * @defgroup timer_hw
 * Definiciones y declaraciones para el módulo de timer por hardware.
 * Este archivo contiene las declaraciones de las funciones utilizadas para
 * configurar e inicializar los timers por hardware.
 * @{
 * @file timer_hw.h
 */
#ifndef TIMER_HW_H
#define TIMER_HW_H

#include <stdint.h>
#include <timer.h>
#include <func_queue.h>

/**
 * @brief Configurar e inicializa los timers por hardware.
 * Timer A0 y A1 con ACLK y distintos targets CCR (up mode) para cada uno. Timer A1 permanece apagado hasta que se llame a @c enable_timer_A1() .
 */
void init_timer_hw(void);

/**
 * @brief Deshabilitar las interrupciones de Timer A0.
 */
void disable_interrupt_timerhw(void);

/**
* @brief Habilitar las interrupciones de Timer A0.
*/
void enable_interrupt_timerhw(void);

/**
 * @brief Establece qué función se invocará cuando se alcance el target del Timer A1.
 * 
 * @param callback Puntero a la función correspondiente.
 */
void set_timer_A1_callback(func* callback);

/**
 * @brief Habilitar Timer A1.
 * 
 * @note TimerA1 se deshabilita cuando interrumpe.
 */
void enable_timer_A1(void);

#endif /* TIMER_HW_H */

/** @} */
