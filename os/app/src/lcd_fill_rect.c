#include "lcd.h"

void lcd_fill_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
{
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            lcd_draw(lcd, x + j, y + i, value);
        }
    }
}
