#include <stdint.h>
#include "display.h"
#include "i2c.h"

#define SSD1306_I2C_ADDRESS             0x3C
#define SSD1306_LCDWIDTH                128
#define SSD1306_LCDHEIGHT               64

#define SSD1306_SETCONTRAST             0x81
#define SSD1306_DISPLAYALLON_RESUME     0xA4
#define SSD1306_DISPLAYALLON            0xA5
#define SSD1306_NORMALDISPLAY           0xA6
#define SSD1306_INVERTDISPLAY           0xA7
#define SSD1306_DISPLAYOFF              0xAE
#define SSD1306_DISPLAYON               0xAF
#define SSD1306_SETDISPLAYOFFSET        0xD3
#define SSD1306_SETCOMPINS              0xDA
#define SSD1306_SETVCOMDETECT           0xDB
#define SSD1306_SETDISPLAYCLOCKDIV      0xD5
#define SSD1306_SETPRECHARGE            0xD9
#define SSD1306_SETMULTIPLEX            0xA8
#define SSD1306_SETLOWCOLUMN            0x00
#define SSD1306_SETHIGHCOLUMN           0x10
#define SSD1306_SETSTARTLINE            0x40
#define SSD1306_MEMORYMODE              0x20
#define SSD1306_COLUMNADDR              0x21
#define SSD1306_PAGEADDR                0x22
#define SSD1306_COMSCANINC              0xC0
#define SSD1306_COMSCANDEC              0xC8
#define SSD1306_SEGREMAP                0xA0
#define SSD1306_CHARGEPUMP              0x8D
#define SSD1306_EXTERNALVCC             0x1
#define SSD1306_SWITCHCAPVCC            0x2
// Scroll functionality.
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A

void command(uint8_t comando) {
    uint8_t com[3];
    com[0] = 0x80;
    com[1] = comando;
    com[2] = 0x00;
    send_message(com);
}

void init_display() {
        // SSD1306 init sequence
    command(SSD1306_DISPLAYOFF);                                // 0xAE
    command(SSD1306_SETDISPLAYCLOCKDIV);                        // 0xD5
    command(0x80);                                              // the suggested ratio 0x80

    command(SSD1306_SETMULTIPLEX);                              // 0xA8
    command(SSD1306_LCDHEIGHT - 1);

    command(SSD1306_SETDISPLAYOFFSET);                          // 0xD3
    command(0x0);                                               // no offset
    command(SSD1306_SETSTARTLINE | 0x0);                        // line #0
    command(SSD1306_CHARGEPUMP);                                // 0x8D
    command(0x14);                                              // generate high voltage from 3.3v line internally
    command(SSD1306_MEMORYMODE);                                // 0x20
    command(0x00);                                              // 0x0 act like ks0108
    command(SSD1306_SEGREMAP | 0x1);
    command(SSD1306_COMSCANDEC);

    command(SSD1306_SETCOMPINS);                                // 0xDA
    command(0x12);
    command(SSD1306_SETCONTRAST);                               // 0x81
    command(0xCF);

    command(SSD1306_SETPRECHARGE);                              // 0xd9
    command(0xF1);
    command(SSD1306_SETVCOMDETECT);                             // 0xDB
    command(0x40);
    command(SSD1306_DISPLAYALLON_RESUME);                       // 0xA4
    command(SSD1306_NORMALDISPLAY);                             // 0xA6

    command(SSD1306_DEACTIVATE_SCROLL);

    command(SSD1306_DISPLAYON);  
}
