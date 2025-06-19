/**
 * @file gui.h
 * @brief Módulo para mostrar elementos en pantalla.
 */
#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <game_data.h>

/**
 * Representación de rango unidimensional.
 * Mínimo permitido: 0.
 * Máximo permitido: 15.
 * @note Para reducir el tamaño, se usan solo 4 bits para start y para end.
 */
typedef struct {
    uint8_t start : 4;
    uint8_t end   : 4;
} template_range_t;

/**
 * Información de contador usado por la GUI.
 * Los digitos más significativos van primero en el arreglo @c digits.
 */
typedef struct {
    /** Cantidad de dígitos del contador. Debe coincidir con el tamaño del arreglo @c digits. */
    const uint8_t digit_amount;
    /** Arreglo de caracteres (un char por digito). */
    uint8_t* digits;
} gui_counter_t;

/**
 * @brief Inicializar interfaz de juego.
 */
void init_gui(void);

/**
 * @brief Renderizar y enviar flecha al display.
 *
 * @param arrow_data  Los datos globales de la flecha a dibujar.
 * @param height      La altura actual de la flecha (entre @c STARTING_ARROW_HEIGHT_LEFT_RIGHT y 127 para derecha/izquiera y entre @c STARTING_ARROW_HEIGHT_UP_DOWN y 127 para arriba/abajo).
 * @param is_outline  Determina si dibujar el contorno o la flecha.
 *
 * @pre @c height está en el rango permitido.
 */
void render_arrow(global_arrow_data_t* arrow_data, uint8_t height, uint8_t is_outline);

/**
 * @brief Limpia un rango determinado de páginas y columnas en la pantalla.
 *
 * @param min_page   Primera página (límite derecho).
 * @param max_page   Última página (límite izquierdo).
 * @param min_column Primera columna (limite superior).
 * @param max_column Última columna (limite inferior).
 * @pre min_page <= max_column, min_column <= max_column, max_column < 128, max_page < 4.
*/
void clean_range(uint8_t min_page, uint8_t max_page, uint8_t min_column, uint8_t max_column);

/**
 * @brief Limpiar posición donde había flecha en el display.
 *
 * @param arrow_data Los datos globales de la flecha a limpiar.
 * @param height     La altura actual de la flecha (entre @c STARTING_ARROW_HEIGHT_LEFT_RIGHT y 127 para derecha/izquiera y entre @c STARTING_ARROW_HEIGHT_UP_DOWN y 127 para arriba/abajo).
 * @param fast_clean Si vale 1, limpia solo la posición anterior a la altura de la flecha. En caso contrario limpia toda la flecha.
 */
void clean_arrow(global_arrow_data_t* arrow_data, uint8_t height, uint8_t fast_clean);

/**
 * @brief Imprime arreglo de caracteres en una posición dada del display.
 *
 * @param buff El arreglo de caracteres.
 * @param size El tamaño del arreglo.
 * @param x    Posición horizontal de la esquina superior izquierda del primer caracter. Crece de izquierda a derecha.
 * @param y    Posición vertical de la esquina superior izquierda del primer caracter. Crece de arriba a abajo.
 *
 * @pre y + altura de las letras (5) <= 128.
 * @pre x + 4*size - 1 <= 32.
 * @pre Valores ASCII válidos: entre ! y Z (no permite letras minúsculas). Sea b un byte de @c buff, entonces: 33 <= b <= 90.
 */
void render_chars(uint8_t* buff, uint8_t size, uint8_t x, uint8_t y);

#endif /* GUI_H */
