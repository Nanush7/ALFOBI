#include "flash.h"
#include <game_data.h>
#include <msp430.h>

sized_array_t* flash_score = (sized_array_t*) 0xC000;

void init_flash() {
    FCTL2 |= FWKEY | FN5 | FN4 | FN2 | FN1 | FN0;
}

void get_flash_scores(sized_array_t* res) {
    *res = *flash_score;
}

void store_scores_to_flash(sized_array_t* scores) {
    FCTL3 = FWKEY;        /* Clear Lock bit */
    FCTL1 = FWKEY + WRT;  /* Set WRT bit for write operation */

    *flash_score = *scores;

    FCTL1=FWKEY;
    FCTL3=FWKEY + LOCK;

    __no_operation();
}


