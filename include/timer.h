/**
 * @file timer.h
 * @brief Proporciona funciones y una estructura para gestionar el tiempo.
 * 
 * Este archivo contiene la declaración de las funciones y la estructura necesaria para
 * gestionar el tiempo en formato horas, minutos, segundos y milisegundos.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#define TIMER_INTERVAL 250 // En caso de cambiarse esto, se debería modificar el código del timer_hw, al igual

/*
 * El timer se inicializa en 0 por defecto en el archivo de implementación.
 */

/**
 * @brief Estructura que representa el tiempo.
 * 
 * La estructura `time_t` contiene los campos necesarios para representar un
 * tiempo en horas, minutos, segundos y milisegundos.
 */
typedef struct {
    uint8_t hours;          /**< Hora en formato 24 horas (0-23). */
    uint8_t minutes;        /**< Minutos (0-59). */
    uint8_t seconds;       /**< Segundos (0-59). */
    uint16_t miliseconds;    /**< Milisegundos (0-999). */
} time_t;

/**
 * @brief Establece el tiempo inicial.
 * 
 * Esta función establece el tiempo a un valor específico, proporcionando
 * una estructura `time_t` con horas, minutos, segundos y milisegundos.
 * 
 * @param time Estructura que contiene la hora, minutos, segundos y milisegundos a establecer.
 */
void set_time(time_t* time);

/**
 * @brief Incrementa el tiempo en 250 milisegundos.
 * 
 * Esta función incrementa el tiempo en la variable global `tiempo_actual` por 250 milisegundos.
 * Si el valor de los milisegundos alcanza 1000, se incrementa el contador de segundos, minutos
 * y horas según corresponda, con los límites de cada unidad de tiempo.
 */
void inc_time();

/**
 * @brief Obtiene el tiempo actual.
 * 
 * Esta función obtiene el valor actual del tiempo y lo copia en una estructura `time_t` proporcionada.
 * 
 * @param buff_time Puntero a la estructura `time_t` en la que se almacenará el tiempo actual.
 */
void get_time(time_t* buff_time);

#endif

