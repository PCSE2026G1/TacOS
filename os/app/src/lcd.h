#ifndef LCD_H
#define LCD_H

/*
 * @startuml(id=lcd)
 * class lcd {
 *     -int display
 *     -unsigned int lcd_pixel(unsigned int x, unsigned int y)
 *     -int get_pixel(const lcd_t* lcd, unsigned int i)
 *     -void set_pixel(lcd_t* lcd, unsigned int i, int value)
 *     -int get_flag(const lcd_t* lcd, unsigned int i)
 *     -void set_flag(lcd_t* lcd, unsigned int i, int value)
 *     +void lcd_draw_string(unsigned int x, unsigned int y, const char* str)
 *     +void lcd_draw_dec(unsigned int x, unsigned int y, int num, unsigned int width)
 *     +void lcd_draw_hex(unsigned int x, unsigned int y, unsigned num, unsigned int width)
 *     +void lcd_set_display(int display)
 *     +int lcd_get_display(void)
 *     +void lcd_init(lcd_t* lcd)
 *     +void lcd_flush(lcd_t* lcd)
 *     +void lcd_draw(lcd_t* lcd, unsigned int x, unsigned int y, int value)
 *     +void lcd_draw_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
 *     +void lcd_fill_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
 * }
 *
 * cmmdef ..> lcd
 * tac_assert ..> lcd
 * tac_string --> lcd
 * @enduml
 */

#include "cmmdef.h"

#ifndef CMMDEF
typedef struct
{
    unsigned int buf[512];
    unsigned int flags[8];
} lcd_t;
#endif

extern void lcd_draw_string(unsigned int x, unsigned int y, const char PTR(str));
extern void lcd_draw_dec(unsigned int x, unsigned int y, int num, unsigned int width);
extern void lcd_draw_hex(unsigned int x, unsigned int y, unsigned int num, unsigned int width);
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
