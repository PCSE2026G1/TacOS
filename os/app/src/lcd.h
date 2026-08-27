#ifndef LCD_H
#define LCD_H

#include "cmmdef.h"

#ifndef CMMDEF
typedef struct
{
    unsigned int buf[512];
    unsigned int flags[8];
} lcd_t;
#endif

extern void lcd_draw_string(unsigned int x, unsigned int y, const char PTR(str));
extern void lcd_draw_dec(unsigned int x, unsigned int y, unsigned int num, unsigned int width);
extern void lcd_set_display(int display);
extern int lcd_get_display(VOID);

extern void lcd_init(STRUCT(lcd_t) PTR(lcd));
extern void lcd_flush(STRUCT(lcd_t) PTR(lcd));
extern void lcd_draw(STRUCT(lcd_t) PTR(lcd),
        unsigned int x, unsigned int y, int value);
extern void lcd_draw_rect(STRUCT(lcd_t) PTR(lcd),
        unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value);
extern void lcd_fill_rect(STRUCT(lcd_t) PTR(lcd),
        unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value);

#endif
