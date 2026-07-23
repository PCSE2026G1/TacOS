#include "lcd.h"
#include "cmm.h"

static int display = 1;

void lcd_draw_string(unsigned int x, unsigned int y, const char* value)
{
    locateXY(x, y);
    putStr(value);
}

void lcd_set_display(int value)
{
    display = value;
}

int lcd_get_display(void)
{
    return display;
}
