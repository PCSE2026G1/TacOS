#include "mahjong.h"
#include "cmmdef.h"
#include "cmm.h"
#include "tac_assert.h"
#include "tac_assert.h"
#include "algorithm.h"
#include "tac_string.h"
#include "key.h"
#include "lcd.h"
#include "random.h"
#include "stacksafe.h"

static int mj_comp_tiles(const void PTR(l), const void PTR(r))
{
    const unsigned char PTR(l2) = l;
    const unsigned char PTR(r2) = r;
    return (l2[0] & 255) - (r2[0] & 255);
}

static void mj_init_tiles(char PTR(tiles))
{
    for (unsigned int i = 0; i < 36; INC(i))
        setp(addp(tiles, i), '1' + (i >> 2));
    shuffle(tiles, 36, 1);
}

static void mj_show_tiles(char PTR(tiles))
{
    char tmp = tiles[13];
    tiles[13] = '\0';
    lcd_draw_string(1, 1, tiles);
    tiles[13] = tmp;
}

static void mj_count_tiles(const char PTR(tiles), int tile, char PTR(counts))
{
    memset(counts, 0, 9);
    for (unsigned int i = 0; i < 13; INC(i))
    {
        unsigned int j = tiles[i] - '1';
        char PTR(p) = addp(counts, j);
        setp(p, getp(p) + 1);
    }
    char PTR(p) = addp(counts, tile - '1');
    setp(p, getp(p) + 1);
}

static int mj_test_hand(const char PTR(tiles), int tile, void PTR(buf))
{
    char PTR(counts) = buf;
    mj_count_tiles(tiles, tile, counts);
    for (unsigned int i = 0; i < 9; INC(i))
    {
        if (counts[i] < 2)
            continue;
        char PTR(p) = addp(counts, i);
        setp(p, getp(p) - 2);
        unsigned int rem = 4;
        for (unsigned int j = 0; j < 9;)
        {
            if (!COND(counts[j]))
            {
                INC(j);
                continue;
            }
            if (counts[j] >= 3)
            {
                char PTR(p2) = addp(counts, j);
                setp(p2, getp(p2) - 3);
                DEC(rem);
            }
            else if (j < 7 && counts[j + 1] >= 1 && counts[j + 2] >= 1)
            {
                for (unsigned int k = 0; k < 3; INC(k))
                {
                    char PTR(p2) = addp(counts, j + k);
                    setp(p2, getp(p2) - 1);
                }
                DEC(rem);
            }
            else
                break;
            if (!COND(rem))
                return 1;
        }
        assert(COND(rem));
        mj_count_tiles(tiles, tile, counts);
    }
    for (unsigned int i = 0; i < 9; INC(i))
        if (counts[i] != 0 && counts[i] != 2)
            return 0;
    return 1;
}

static unsigned int mj_tenpai(const void PTR(tiles), void PTR(buf))
{
    unsigned int bits = 0;
    for (int tile = '1'; tile <= '9'; INC(tile))
    {
        if (COND(mj_test_hand(tiles, tile, buf)))
        {
            ORA(bits, 1 << (tile - '1'));
        }
    }
    return bits;
}

static void mj_show_tenpai(unsigned int x, unsigned int y, unsigned int bits, void PTR(buf))
{
    char PTR(p) = buf;
    memset(p, ' ', 9);
    p[10] = '\0';
    for (unsigned int i = 0; i < 9; INC(i))
    {
        if (COND(bits >> i & 1))
        {
            setp(p, i + '1');
            p = addp(p, 1);
        }
    }
    lcd_draw_string(x, y, buf);
}

static void play(void PTR(buf))
{
    char PTR(tiles) = buf;
    void PTR(buf2) = addp(buf, 36);
    key_step();
    unsigned int game = 0;
    unsigned int score = 0;
    do
    {
        mj_init_tiles(tiles);
        hsort(tiles, 13, 1, F_ADDR(mj_comp_tiles));
        lcd_draw_string(0, 0, "                ");
        lcd_draw_string(0, 1, " 123456789ABCD  ");
        lcd_draw_string(0, 2, "                ");
        lcd_draw_string(0, 3, " >              ");
        lcd_draw_string(0, 4, "                ");
        lcd_draw_string(0, 5, " *Select <1>    ");
        lcd_draw_string(0, 6, "                ");
        lcd_draw_string(0, 7, "  OK       99999");
        mj_show_tiles(tiles);
        lcd_draw_dec(11, 7, score, 5);
        unsigned int cursor = 0;
        int selected = '1';
        unsigned int selected_bits = 0;
        do
        {
            key_step();
            if (COND(key_pressed(KEY_BACK)))
                return;
            unsigned int cursor0 = cursor;
            if (COND(key_pressed(KEY_UP)) && cursor > 0)
            {
                DEC(cursor);
            }
            if (COND(key_pressed(KEY_DOWN)) && cursor < 1)
            {
                INC(cursor);
            }
            if (cursor != cursor0)
            {
                setp(buf2, ' ');
                setp(addp(buf2, 1), '\0');
                lcd_draw_string(1, 5 + cursor0 * 2, buf2);
                setp(buf2, '*');
                lcd_draw_string(1, 5 + cursor * 2, buf2);
                if (cursor == 0)
                {
                    setp(buf2, '<');
                    lcd_draw_string(9, 5, buf2);
                    setp(buf2, '>');
                    lcd_draw_string(11, 5, buf2);
                }
                else
                {
                    setp(buf2, ' ');
                    lcd_draw_string(9, 5, buf2);
                    lcd_draw_string(11, 5, buf2);
                }
            }
            int selected0 = selected;
            if (COND(key_pressed(KEY_LEFT)) && cursor == 0)
            {
                if (selected > '1')
                    DEC(selected);
                else
                    selected = '9';
            }
            if (COND(key_pressed(KEY_RIGHT)) && cursor == 0)
            {
                if (selected < '9')
                    INC(selected);
                else
                    selected = '1';
            }
            if (selected != selected0)
            {
                setp(buf2, selected);
                setp(addp(buf2, 1), '\0');
                lcd_draw_string(10, 5, buf2);
            }
            if (COND(key_pressed(KEY_ENTER)))
            {
                if (cursor == 0)
                {
                    XORA(selected_bits, 1 << (selected - '1'));
                    mj_show_tenpai(3, 3, selected_bits, buf2);
                }
                else
                {
                    unsigned int tenpai = mj_tenpai(tiles, buf2);
                    lcd_draw_string(1, 2, "Selected");
                    lcd_draw_string(1, 4, "Answer");
                    lcd_draw_string(1, 5, "> ");
                    mj_show_tenpai(3, 5, tenpai, buf2);
                    if (selected_bits == tenpai)
                    {
                        lcd_draw_string(1, 7, " :)");
                        INC(score);
                        lcd_draw_dec(11, 7, score, 5);
                    }
                    else
                    {
                        lcd_draw_string(1, 7, " :(");
                        game = 1;
                    }
                    do
                    {
                        sleep(1);
                        key_step();
                    } while (!COND(key_pressed(KEY_ENTER)));
                    break;
                }
            }
            llrand();
            sleep(1);
        } while (!COND(game));
    } while (!COND(game));
}

extern void mj_play(VOID)
{
    void PTR(buf) = malloc(512);
    assert(buf != NULL);
    STACKSAFE(buf, 512, play, 1, buf);
    free(buf);
}
