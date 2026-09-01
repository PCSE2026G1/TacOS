#include "minesweeper.h"
#include "minesweeper0.h"
#include "cmmdef.h"
#include "cmm.h"
#include "string.h"
#include "random.h"
#include "algorithm.h"
#include "lcd.h"
#include "key.h"
#include "stacksafe.h"

static void PTR(ms_ptr(const void PTR(buf), unsigned int x, unsigned int y))
{
    return addp(buf, y * 8 + x);
}

static unsigned int ms_count(const void PTR(buf),
        unsigned int x, unsigned int y, unsigned int mask, unsigned int value)
{
    unsigned int i1 = cond3(y < 1, 0, y - 1);
    unsigned int i2 = cond3(y >= 7, 8, y + 2);
    unsigned int j1 = cond3(x < 1, 0, x - 1);
    unsigned int j2 = cond3(x >= 7, 8, x + 2);

    unsigned int count = 0;
    for (unsigned int i = i1; i < i2; INC(i))
    {
        for (unsigned int j = j1; j < j2; INC(j))
        {
            if ((getp(ms_ptr(buf, j, i)) & mask) == value)
                INC(count);
        }
    }

    return count;
}

static void ms_init(void PTR(buf), unsigned int x, unsigned int y)
{
    if (x < 1)
        x = 1;
    if (x > 6)
        x = 6;
    if (y < 1)
        y = 1;
    if (y > 6)
        y = 6;

    for (unsigned int i = 0; i < MS_COUNT; INC(i))
        setp(addp(buf, i), 10);
    for (unsigned int i = MS_COUNT; i < 64; INC(i))
        setp(addp(buf, i), 0);

    for (unsigned int i = 0; i < 55; INC(i))
    {
        unsigned int j = ((llrand() >> 6) * 55) >> 9;
        swap(addp(buf, i), addp(buf, j), 1);
    }

    for (unsigned int i = 0; i < 3; INC(i))
    {
        for (unsigned int j = 0; j < 3; INC(j))
        {
            swap(ms_ptr(buf, x + j - 1, y + i - 1), addp(buf, i * 3 + j + 55), 1);
        }
    }

    for (unsigned int i = 0; i < 8; INC(i))
    {
        for (unsigned int j = 0; j < 8; INC(j))
        {
            void PTR(p) = ms_ptr(buf, j, i);
            if (getp(p) != MS_MINE_MASK)
                setp(p, ms_count(buf, j, i, MS_NUM_MASK, MS_MINE_MASK));
        }
    }
}

static unsigned int ms_open(void PTR(buf), unsigned int x, unsigned int y)
{
    void PTR(p) = ms_ptr(buf, x, y);
    unsigned int value = getp(p);
    unsigned int num = value & MS_NUM_MASK;

    if (COND(value & MS_FLAGGED_MASK))
        return 0;

    unsigned int count = 0;
    if (!COND(value & MS_OPENED_MASK))
    {
        ORA(value, MS_OPENED_MASK);
        setp(p, value);

        count = cond3(num == MS_MINE_MASK, 64, 1);
    }

    if (ms_count(buf, x, y, MS_FLAGGED_MASK, MS_FLAGGED_MASK) == num)
    {
        unsigned int i1 = cond3(y < 1, 0, y - 1);
        unsigned int i2 = cond3(y >= 7, 8, y + 2);
        unsigned int j1 = cond3(x < 1, 0, x - 1);
        unsigned int j2 = cond3(x >= 7, 8, x + 2);

        for (unsigned int i = i1; i < i2; INC(i))
        {
            for (unsigned int j = j1; j < j2; INC(j))
            {
                if (!COND(getp(ms_ptr(buf, j, i)) & MS_OPENED_MASK))
                    ADDA(count, ms_open(buf, j, i));
            }
        }
    }

    return count;
}

static unsigned int ms_x(unsigned int j)
{
    return j * 7 + 36;
}
static unsigned int ms_y(unsigned int i)
{
    return i * 7 + 4;
}

static void ms_draw(STRUCT(lcd_t) PTR(lcd), const void PTR(buf),
        unsigned int cx, unsigned int cy, unsigned int rem)
{
    lcd_fill_rect(lcd, 36, 4, 56, 56, 0);

    for (unsigned int i = 0; i < 8; INC(i))
    {
        for (unsigned int j = 0; j < 8; INC(j))
        {
            unsigned int value = getp(ms_ptr(buf, j, i));
            unsigned int x = ms_x(j) + 1;
            unsigned int y = ms_y(i) + 1;

            if (COND(value & MS_FLAGGED_MASK))
            {
                ms_draw_icon(lcd, x, y, 11);
            }
            else if (COND(value & MS_OPENED_MASK))
            {
                ms_draw_icon(lcd, x, y, value & MS_NUM_MASK);
            }
            else
            {
                lcd_fill_rect(lcd, x, y, 5, 5, 1);
            }
        }
    }

    lcd_draw_rect(lcd, ms_x(cx), ms_y(cy), 7, 7, 1);

    lcd_flush(lcd);

    if (rem >= 1 && rem <= 64)
        lcd_draw_dec(14, 7, rem, 2);
    else if (rem == 0)
        lcd_draw_string(14, 7, ":)");
    else
        lcd_draw_string(14, 7, ":(");
}

static void play(void PTR(buf))
{
    STRUCT(lcd_t) PTR(lcd) = addp(buf, 64);
    memset(buf, 0, 64);
    lcd_fill_rect(lcd, 0, 0, 128, 64, 0);
    lcd_draw_rect(lcd, 35, 3, 58, 58, 1);
    key_step();

    unsigned int x = 1;
    unsigned int y = 1;
    unsigned int rem = 64 - MS_COUNT;
    int update = 1;
    do
    {
        if (rem == 64 - MS_COUNT)
        {
#ifndef DEBUG
            llrand();
#endif
        }

        if (COND(update))
        {
            ms_draw(lcd, buf, x, y, rem);
            update = 0;
        }
        else
        {
            sleep(1);
        }

#ifdef DEBUG
        lcd_draw_hex(14, 6, key_get(), 2);
#endif

        unsigned int x0 = x;
        unsigned int y0 = y;
        key_step();
        if (COND(key_pressed(KEY_UP)) && y >= 1)
            DEC(y);
        if (COND(key_pressed(KEY_LEFT)) && x >= 1)
            DEC(x);
        if (COND(key_pressed(KEY_RIGHT)) && x < 7)
            INC(x);
        if (COND(key_pressed(KEY_DOWN)) && y < 7)
            INC(y);
        if (x != x0 || y != y0)
            update = 1;

        if (COND(key_pressed(KEY_BACK)))
        {
            void PTR(p) = ms_ptr(buf, x, y);
            setp(p, getp(p) ^ MS_FLAGGED_MASK);
            update = 1;
        }
        if (COND(key_pressed(KEY_ENTER)))
        {
            if (rem == 64 - MS_COUNT)
                ms_init(buf, x, y);

            SUBA(rem, ms_open(buf, x, y));
            update = 1;
        }
    } while (rem >= 1 && rem <= 64);

    for (unsigned int i = 0; i < 8; INC(i))
    {
        for (unsigned int j = 0; j < 8; INC(j))
        {
            void PTR(p) = ms_ptr(buf, j, i);
            setp(p, (getp(p) | MS_OPENED_MASK) & ~MS_FLAGGED_MASK);
        }
    }
    ms_draw(lcd, buf, x, y, rem);

    do
    {
        key_step();
    } while (!COND(key_pressed(KEY_ENTER)));
}

extern void ms_play(VOID)
{
    void PTR(buf) = malloc(1536);
    stacksafe(buf, 1536, INT_ADDR(play), 1, buf);
    free(buf);
}
