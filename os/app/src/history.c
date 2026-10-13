#include "history.h"

#include "cmmdef.h"
#include "cmm.h"
#include "key.h"
#include "lcd.h"
#include "playlist.h"
#include <PlayHistory.hmm>

#define HISTORY_NAME_SIZE 11
#define HISTORY_VISIBLE 5

static int history_same_path(const char PTR(l), const char PTR(r))
{
    if (l == NULL || r == NULL)
        return 0;
    for (unsigned int i = 0;; INC(i))
    {
        if (l[i] != r[i])
            return 0;
        if (l[i] == '\0')
            return 1;
    }
    return 0;
}

/* PLAYHIST.TXT の同じ曲について，最後に記録された要素だけを表示する． */
static int history_has_later(SPTR(playlist_item_t, item))
{
    const char PTR(path) = MEMBER(item, name);
    for (item = MEMBER(item, next); item != NULL; item = MEMBER(item, next))
        if (COND(history_same_path(path, MEMBER(item, name))))
            return 1;
    return 0;
}

static unsigned int history_unique_count(SPTR(playlist_t, history))
{
    unsigned int count = 0;
    for (SPTR(playlist_item_t, item) = MEMBER(history, back);
            item != NULL; item = MEMBER(item, prev))
        if (!COND(history_has_later(item)))
            INC(count);
    return count;
}

/* 新しく再生された曲から順に index 番目を返す． */
static SPTR(playlist_item_t, history_item_at(
        SPTR(playlist_t, history), unsigned int index))
{
    unsigned int current = 0;
    for (SPTR(playlist_item_t, item) = MEMBER(history, back);
            item != NULL; item = MEMBER(item, prev))
    {
        if (!COND(history_has_later(item)))
        {
            if (current == index)
                return item;
            INC(current);
        }
    }
    return NULL;
}

static unsigned int history_play_count(
        SPTR(playlist_t, history), const char PTR(path))
{
    unsigned int count = 0;
    for (SPTR(playlist_item_t, item) = MEMBER(history, front);
            item != NULL; item = MEMBER(item, next))
        if (COND(history_same_path(path, MEMBER(item, name))))
            INC(count);
    return count;
}

static void history_clear(VOID)
{
    for (unsigned int y = 0; y < 8; INC(y))
        lcd_draw_string(0, y, "                ");
}

static int history_mp3_extension(const char PTR(name), unsigned int length)
{
    if (length < 4 || name[length - 4] != '.' || name[length - 1] != '3')
        return 0;
    int m = name[length - 3];
    int p = name[length - 2];
    if ((m == 'M' || m == 'm') && (p == 'P' || p == 'p'))
        return 1;
    return 0;
}

static void history_short_name(const char PTR(path), char PTR(dest))
{
    const char PTR(name) = path;
    for (unsigned int i = 0; path[i] != '\0'; INC(i))
        if (path[i] == '/')
            name = addp(path, i + 1);

    unsigned int length = strLen(name);
    if (COND(history_mp3_extension(name, length)))
        SUBA(length, 4);
    if (length >= HISTORY_NAME_SIZE)
        length = HISTORY_NAME_SIZE - 1;

    for (unsigned int i = 0; i < length; INC(i))
        dest[i] = name[i];
    dest[length] = '\0';
}

static void history_draw(SPTR(playlist_t, history), unsigned int cursor)
{
    history_clear();
    lcd_draw_string(0, 0, "# HISTORY #");

    unsigned int count = history_unique_count(history);
    if (count == 0)
    {
        lcd_draw_string(1, 2, "(empty)");
    }
    else
    {
        unsigned int start = 0;
        if (cursor >= HISTORY_VISIBLE)
            start = cursor - HISTORY_VISIBLE + 1;

        char PTR(name) = malloc(HISTORY_NAME_SIZE);
        if (name != NULL)
        {
            for (unsigned int row = 0;
                    row < HISTORY_VISIBLE && start + row < count; INC(row))
            {
                SPTR(playlist_item_t, item) = history_item_at(history, start + row);
                if (item == NULL)
                    break;
                if (start + row == cursor)
                    lcd_draw_string(0, row + 1, "*");
                else
                    lcd_draw_string(0, row + 1, " ");

                const char PTR(path) = MEMBER(item, name);
                history_short_name(path, name);
                lcd_draw_string(1, row + 1, name);
                lcd_draw_string(12, row + 1, "x");
                unsigned int shown = history_play_count(history, path);
                if (shown > 999)
                    shown = 999;
                lcd_draw_dec(13, row + 1, shown, 3);
            }
            free(name);
        }
    }

    lcd_draw_string(0, 6, "UP/DN:Select");
    lcd_draw_string(0, 7, "ENT:Play BACK");
}

static void history_wait_release(VOID)
{
    while (COND(key_read()))
        sleep(10);
    key_step();
    key_step();
}

extern void history_show(VOID)
{
    SPTR(playlist_t, history) = pl_alloc();
    if (history == NULL)
        return;

    /* 履歴ファイルがまだ無い場合は，空の履歴として表示する． */
    pl_read(history, PLAY_HISTORY_FILE);

    unsigned int cursor = 0;
    history_wait_release();
    history_draw(history, cursor);

    while (COND(1))
    {
        key_step();
        unsigned int count = history_unique_count(history);
        if (COND(key_pressed(KEY_UP)) && cursor > 0)
        {
            DEC(cursor);
            history_draw(history, cursor);
        }
        else if (COND(key_pressed(KEY_DOWN)) && cursor + 1 < count)
        {
            INC(cursor);
            history_draw(history, cursor);
        }
        else if (COND(key_pressed(KEY_ENTER)) && count > 0)
        {
            SPTR(playlist_item_t, item) = history_item_at(history, cursor);
            if (item != NULL)
            {
                const char PTR(path) = MEMBER(item, name);
                history_wait_release();
                mp3_play_path(path);
                history_wait_release();
                pl_add(history, path);
                cursor = 0;
                history_draw(history, cursor);
            }
        }
        else if (COND(key_pressed(KEY_BACK)))
        {
            pl_free(history);
            return;
        }
        sleep(10);
    }
}
