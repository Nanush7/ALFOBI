#include "intrinsics.h"
#include "stream.h"
#include <i2c.h>
#include <msp430.h>
#include <string.h>

int main() {
    /* Paramos el gordo perro guardián. */
    WDTCTL = WDTPW + WDTHOLD;

    init_i2c(0);
    __enable_interrupt();

    /* Esperamos por alguna razón que no será aclarada acá. */
    __delay_cycles(1000);


    uint8_t message[20] = "hola";
    send_message(message);

    strcpy(message, "buenas");
    send_message(message);

    strcpy(message, "adios");
    while(1) {
        if (stream_free_space() > 10) {
            send_message(message);
        } 
    }

    return 0;
}
