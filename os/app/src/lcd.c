#include "lcd.h"

#include "cmmdef.h"
#include "cmm.h"
#include "assert.h"
#include "memset.h"

static int display = 1;

extern void lcd_draw_string(unsigned int x, unsigned int y, const char PTR(str))
{
    locateXY(x, y);
    putStr(str);
}

extern void lcd_draw_dec(unsigned int x, unsigned int y, int num, unsigned int width)
{
#ifdef LCD_ASSERT_UB
    assert(num != -32768);
#endif
    assert(cmpu(x, 16) < 0);
    assert(cmpu(x + width, 16) <= 0);
    assert(cmpu(y, 8) < 0);
    unsigned int size = 7;
    if (width > size)
        size = width;
    char PTR(buf) = malloc(size + 1);
    buf[size] = '\0';
    int sign = 0;
    if (num < 0)
    {
        sign = 1;
        num = -num;
    }
    unsigned int i = size;

    do
    {
        DEC(i);
        int ch = 0x30 + (num % 10);
        setp(addp(buf, i), ch);
        DIVA(num, 10);
    } while (COND(num));

    if (COND(sign))
    {
        DEC(i);
        buf[i] = '-';
    }

    while (cmpu(i, size - width) > 0)
    {
        DEC(i);
        buf[i] = ' ';
    }

    lcd_draw_string(x, y, addp(buf, i));
    free(buf);
}

extern void lcd_set_display(int x)
{
    if (COND(x))
        x = 1;

    char PTR(buf) = malloc(2);
    setp(buf, x | 0xae);
    buf[1] = '\xff';
    spiWriteLcdCom(buf);
    free(buf);
    display = x;
}

extern int lcd_get_display(VOID)
{
    return display;
}

extern void lcd_init(STRUCT(lcd_t) PTR(lcd))
{
    assert(lcd != NULL);
    memset(addp(lcd, 1024), 0, 16);
}

extern void lcd_flush(STRUCT(lcd_t) PTR(lcd))
{
    assert(lcd != NULL);
    void PTR(fbuf) = addp(lcd, 1024);
    for (unsigned int y = 0; y < 8; INC(y))
    {
        void PTR(p) = addp2(fbuf, y);
        unsigned int flags = getp2(p);
        for (unsigned int x = 0; x < 16; INC(x))
        {
            if (COND((flags >> x) & 1))
            {
                locateXY(x, y);
                spiWriteLcdDat(addp(lcd, (y * 16 + x) * 8), 8);
            }
        }
        setp2(p, 0);
    }
}

static unsigned int lcd_pixel(unsigned int x, unsigned int y)
{
    unsigned int i = y * 128 + x;
    assert(cmpu(i, 128 * 64) < 0);
    x = (i >> 3) & 15;
    y = (i >> 10) & 7;
    i = ((i & 7) << 3) | ((i >> 7) & 7);
    return i | (x << 6) | (y << 10);
}

static int get_pixel(const STRUCT(lcd_t) PTR(lcd), unsigned int i)
{
    assert(lcd != NULL);
    const void PTR(p) = addp(lcd, i >> 3);
    return (getp(p) >> (i & 7)) & 1;
}

static void set_pixel(STRUCT(lcd_t) PTR(lcd), unsigned int i, int value)
{
    assert(lcd != NULL);
    void PTR(p) = addp(lcd, i >> 3);
    unsigned int mask = 1 << (i & 7);
    unsigned int pixels = getp(p);
    if (COND(value))
    {
        ORA(pixels, mask);
    }
    else
    {
        ANDA(pixels, ~mask);
    }
    setp(p, pixels);
}

static int get_flag(const STRUCT(lcd_t) PTR(lcd), unsigned int i)
{
    assert(lcd != NULL);
    SHRA(i, 6);
    assert(cmpu(i >> 4, 8) < 0);
    const void PTR(p) = addp2(lcd, 512 + (i >> 4));
    return (getp2(p) >> (i & 15)) & 1;
}

static void set_flag(STRUCT(lcd_t) PTR(lcd), unsigned int i, int value)
{
    assert(lcd != NULL);
    SHRA(i, 6);
    assert(cmpu(i >> 4, 8) < 0);
    void PTR(p) = addp2(lcd, 512 + (i >> 4));
    unsigned int flags = getp2(p);
    unsigned int mask = 1 << (i & 15);
    if (COND(value))
    {
        ORA(flags, mask);
    }
    else
    {
        ANDA(flags, ~mask);
    }
    setp2(p, flags);
}

extern void lcd_draw(STRUCT(lcd_t) PTR(lcd), unsigned int x, unsigned int y, int value)
{
    unsigned int i = lcd_pixel(x, y);
    set_pixel(lcd, i, value);
    set_flag(lcd, i, 1);
}

extern void lcd_draw_rect(STRUCT(lcd_t) PTR(lcd), unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
{
    if (width >= 1 && height >= 1)
    {
        lcd_fill_rect(lcd, x, y, width, 1, value);
        lcd_fill_rect(lcd, x, y + height - 1, width, 1, value);
        if (height >= 3)
        {
            lcd_fill_rect(lcd, x, y + 1, 1, height - 2, value);
            lcd_fill_rect(lcd, x + width - 1, y + 1, 1, height - 2, value);
        }
    }
}

extern void lcd_fill_rect(STRUCT(lcd_t) PTR(lcd), unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
{
    assert(lcd != NULL);
    assert(cmpu(x, 128) < 0);
    assert(cmpu(y, 64) < 0);
    assert(cmpu(x + width, 128) <= 0);
    assert(cmpu(y + height, 64) <= 0);

    for (unsigned int i = 0; i < height; INC(i))
    {
        for (unsigned int j = 0; j < width; INC(j))
        {
            lcd_draw(lcd, x + j, y + i, value);
        }
    }
}
