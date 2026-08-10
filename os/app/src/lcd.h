#ifndef LCD_H
#define LCD_H

#include "cmmdef.h"

extern void lcd_draw_string(unsigned int x, unsigned int y, const char PTR(str));
extern void lcd_set_display(int display);
extern int lcd_get_display(VOID);

#endif
