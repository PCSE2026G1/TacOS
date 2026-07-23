#ifndef LCD_H
#define LCD_H

void lcd_draw_string(unsigned int x, unsigned int y, const char* value);
void lcd_set_display(int display);
int lcd_get_display(void);

#endif
