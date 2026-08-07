#include "lcd.h"
#include "cmm.h"

#define LCD_DISPLAY_MASK 0xae

static int display = 1;

void lcd_draw_string(unsigned int x, unsigned int y, const char* value)
{
    locateXY(x, y);
    putStr(value);
}

void lcd_set_display(int value)
{
    value = value ? 1 : 0;

    unsigned char buf[] = {LCD_DISPLAY_MASK, 0xff};
    if (value)
        buf[0] |= 1;
    spiWriteLcdCom(buf);

    display = value;
}

int lcd_get_display(void)
{
    return display;
}
