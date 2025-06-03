#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/**
 * @brief Inicializar display SSD1306 y apagar/borrar todos los pixeles.
 */
void init_display(void);

/**
 * @brief Limpia el display en el área comprendida entre el page_start, el column_start y el fin del display.
 *
 * @param page_start Desde qué página comenzar a limpiar.
 * @param column_start Desde qué columna comenzar a limpiar.
 */
void clear_area(uint8_t page_start, uint8_t column_start);

/**
 * @brief Mover el puntero a la página y columna especificados. Procedimiento privado.
 *
 * @param page_start   La página seleccionada.
 * @param column_start La columna seleccionada.
 *
 * @warning Selecciona todas las páginas y columnas desde las indicadas hasta el final del display (hasta 3 y 127 respectivamente).
 */
void set_cursor_position(uint8_t page_start, uint8_t column_start);

/**
 * @brief Escribe el dato en el display 
 *
 * @param data valor a ser escrito en el display.
*/
void write_data(uint8_t data);

#endif /* DISPLAY_H */
