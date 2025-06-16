#ifndef FLASH_H
#define FLASH_H
#include <game_data.h>

/** TODO: Doxygen */
void init_flash();

/** Obtiene los puntajes guardados en flash.
 *
 */
void get_flash_scores(scores_t* res);

void store_scores_to_flash(scores_t* scores);

#endif /* FLASH_H */
