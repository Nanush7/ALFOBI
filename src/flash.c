#include "flash.h"
#include <msp430.h>
#include "assert_test.h"
#include "intrinsics.h"

/** Puntero a segmento ALFOBI_SCORES en FLASH. */
scores_t* flash_score = (scores_t*)0xC000;

void init_flash(void) {
    FCTL2 = FWKEY + FN5 + FN4 + FN2 + FN1 + FN0;
}

void get_flash_scores(scores_t* res) {
    *res = *flash_score;
}

void store_scores_to_flash(scores_t* scores) {
    P1OUT |= BIT0;
    __disable_interrupt();

    /* Desbloquear flash */
    FCTL3 = FWKEY;
    /* Modo erase. */
    FCTL1 = FWKEY + ERASE;
    
    /* Borrar segmento apuntado por flash_score (borra 512 bytes).  */
    *(uint16_t*)flash_score = 0xFFFF;

    while(FCTL3 & BUSY);
    
    /* Modo escritura. */
    FCTL1 = FWKEY + WRT;
    
    /* Escribir palabra por palabra (16 bits). */
    uint16_t* src = (uint16_t*)scores;
    uint16_t* dst = (uint16_t*)flash_score;
    static const uint16_t words = sizeof(scores_t) / 2;
    
    for(uint16_t i = 0; i < words; i++) {
        dst[i] = src[i];
        while(FCTL3 & BUSY);
    }
    
    /* Deshabilitar escritura y bloquear flash */
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;

    __enable_interrupt();
    P1OUT &= ~BIT0;
}
