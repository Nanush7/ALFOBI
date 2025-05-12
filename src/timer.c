#include <timer.h>
#include <timer_hw.h>
#include <msp430.h>

#define INCREMENTO 250 /* Incremento de tiempo en milisegundos. */

time_t current_time = {0, 0, 0, 0}; /* Variable global que almacena el tiempo actual. */

/**
 * @brief Establece el tiempo inicial.
 * 
 * Esta función establece el tiempo a un valor específico. Los valores de horas, minutos,
 * segundos y milisegundos se actualizan con los valores proporcionados en la estructura `time_t`.
 * 
 * @param time Estructura que contiene la hora, minutos, segundos y milisegundos a establecer.
 */
void set_time(time_t* time) {
    disable_interrupt_timerhw();
    current_time = *time;
    enable_interrupt_timerhw();
}

/**
 * @brief Incrementa el tiempo en 250 milisegundos.
 * 
 * Esta función incrementa el tiempo en la variable global `current_time` por 250 milisegundos.
 * Si los milisegundos alcanzan 1000, se incrementan los segundos. Cuando los segundos llegan a
 * 60, se incrementan los minutos y, finalmente, cuando los minutos llegan a 60, se incrementan las horas.
 * Si las horas alcanzan 24, se reinicia a 0.
 */
void inc_time() {
    current_time.miliseconds += TIMER_INTERVAL;
    
    // Incremento de segundos si los milisegundos llegan a 1000
    if (current_time.miliseconds < 1000) return;
    current_time.seconds++;
    current_time.miliseconds = 0; // Se asume que siempre aumenta de a 250, en caso contrario habría que tomarle módulo 1000.

    // Incremento de minutos si los segundos llegan a 60
    if (current_time.seconds < 60) return;
    current_time.minutes++;
    current_time.seconds = 0;

    // Incremento de horas si los minutos llegan a 60
    if (current_time.minutes < 60) return;
    current_time.hours++;
    current_time.minutes = 0;

    // Reinicio de horas si se supera 24
    if (current_time.hours >= 24)
        current_time.hours = 0;
}

/**
 * @brief Obtiene el tiempo actual.
 * 
 * Esta función copia el valor actual de la variable global `current_time` en la estructura
 * `buff_time` proporcionada. El valor de horas, minutos, segundos y milisegundos es
 * almacenado en la estructura indicada.
 * 
 * @param buff_time Puntero a la estructura `time_t` en la que se almacenará el tiempo actual.
 */
void get_time(time_t* buff_time) {
    disable_interrupt_timerhw();
    buff_time->hours        = current_time.hours;
    buff_time->minutes      = current_time.minutes;
    buff_time->seconds     = current_time.seconds;
    buff_time->miliseconds = current_time.miliseconds;
    enable_interrupt_timerhw();
}
