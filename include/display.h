/**
 * @defgroup display
 * Módulo para comunicación con display SSD1306.
 * @{
 * @file display.h
 */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/**
 * @brief Inicializa el display OLED SSD1306.
 * Esta función envía la secuencia de comandos necesaria para inicializar
 * el SSD1306.
 */
void init_display(void);

/**
 * @brief Mover cursor del display a la página y columna especificados.
 *
 * @param page_start   La página seleccionada.
 * @param column_start La columna seleccionada.
 *
 * @warning Selecciona todas las páginas y columnas desde las indicadas hasta el final del display (hasta 3 y 127 respectivamente).
 */
void set_cursor_position(uint8_t page_start, uint8_t column_start);

/**
 * @brief Enviar dato al display.
 * Envía un byte CONTROL_DATA y un byte de data.
 *
 * @param data Valor a enviar.
*/
void write_data(uint8_t data);

#endif /* DISPLAY_H */

/** @} */
