#include <msp430.h>
#include "intrinsics.h"
#include "i2c.h"
#include "display.h"

int main() {
    /* Paramos el Watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    init_i2c(0x3C);
    __enable_interrupt();

    init_display();
    while(1);

    return 0;
}
