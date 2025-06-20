/**
 * @addtogroup display
 * @{
 * @file display.c
 */
#include "display.h"
#include "i2c.h"

#define LCDWIDTH             128
#define LCDHEIGHT            32
#define I2C_ADDRESS          0x3C
#define CONTROL_DATA         0xC0
#define CONTROL_COMMAND      0x80

/** Comandos SSD1306. */
typedef enum SSD1306_command {
    SET_CONTRAST            = 0x81,
    DISPLAY_ALL_ON_RESUME   = 0xA4,
    DISPLAY_ALLON           = 0xA5,
    NORMAL_DISPLAY          = 0xA6,
    INVERT_DISPLAY          = 0xA7,
    DISPLAY_OFF             = 0xAE,
    DISPLAY_ON              = 0xAF,
    SET_DISPLAY_OFFSET      = 0xD3,
    SET_COM_PINS            = 0xDA,
    SET_VCOM_DESELECT       = 0xDB,
    SET_DISPLAY_CLOCK_DIV   = 0xD5,
    SET_PRECHARGE           = 0xD9,
    SET_MULTIPLEX           = 0xA8,
    SET_LOW_COLUMN          = 0x00,
    SET_HIGH_COLUMN         = 0x10,
    SET_START_LINE          = 0x40,
    MEMORY_MODE             = 0x20,
    VERTICAL_ADDR_MODE      = 0x2,
    HORIZONTAL_ADDR_MODE    = 0x0,
    COLUMN_ADDR             = 0x21,
    PAGE_ADDR               = 0x22,
    COM_SCAN_INC            = 0xC0,
    COM_SCAN_DEC            = 0xC8,
    SEG_REMAP               = 0xA0,
    CHARGE_PUMP             = 0x8D,
    EXTERNAL_VCC            = 0x1,
    SWITCH_CAP_VCC          = 0x2,
    DEACTIVATE_SCROLL       = 0x2E
} SSD1306_command_t;

/**
 * @brief Enviar comando al display.
 * Envía un byte CONTROL_COMMAND y un byte de comando. Procedimiento privado.
 *
 * @param command El comando a enviar.
 */
void command(uint8_t command) {
    uint8_t com[2];
    com[0] = CONTROL_COMMAND;
    com[1] = command;
    send_message(com);
}

void write_data(uint8_t data) {
    uint8_t com[2];
    com[0] = CONTROL_DATA;
    com[1] = data;
    send_message(com);
}

void init_display(void) {
    command(DISPLAY_OFF);

    command(SET_DISPLAY_CLOCK_DIV);
    command(0x80);

    command(SET_MULTIPLEX);
    command(LCDHEIGHT - 1);

    command(SET_DISPLAY_OFFSET);
    command(0x0);
    command(SET_START_LINE);
    command(CHARGE_PUMP);
    command(0x14);

    command(MEMORY_MODE);
    command(HORIZONTAL_ADDR_MODE);

    command(SEG_REMAP | 0x1);
    command(COM_SCAN_DEC);

    command(SET_COM_PINS);
    command(0x02);

    command(SET_CONTRAST);
    command(0xCF);

    command(SET_PRECHARGE);
    command(0xF1);

    command(SET_VCOM_DESELECT);
    command(0x30);

    command(DISPLAY_ALL_ON_RESUME);

    command(NORMAL_DISPLAY);

    command(DEACTIVATE_SCROLL);

    command(DISPLAY_ON);
}

void set_cursor_position(uint8_t page_start, uint8_t column_start) {
    command(PAGE_ADDR);
    command(page_start);
    command(3);

    command(COLUMN_ADDR);
    command(column_start);
    command(127);
}

/** @} */
