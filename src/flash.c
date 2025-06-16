#include "flash.h"
#include <msp430.h>

scores_t* flash_score = (scores_t*) 0xC000;

void init_flash() {
    FCTL2 = FWKEY + FN5 + FN4 + FN2 + FN1 + FN0;
}

void get_flash_scores(scores_t* res) {
    *res = *flash_score;
}

void store_scores_to_flash(scores_t* scores) {
    __disable_interrupt();
    FCTL3 = FWKEY;        /* Clear Lock bit */
    FCTL1 = FWKEY + WRT;  /* Set WRT bit for write operation */

    *flash_score = *scores;

    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;

    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __enable_interrupt();
}
