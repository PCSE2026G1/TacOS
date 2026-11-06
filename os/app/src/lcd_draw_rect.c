#include "lcd.h"

void lcd_draw_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
{
    lcd_fill_rect(lcd, x, y, width, 1, value);
    lcd_fill_rect(lcd, x, y + height - 1, width, 1, value);
    lcd_fill_rect(lcd, x, y + 1, 1, height - 2, value);
    lcd_fill_rect(lcd, x + width - 1, y + 1, 1, height - 2, value);
}
