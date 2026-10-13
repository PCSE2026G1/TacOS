#include "history.h"

#include "cmmdef.h"
#include "cmm.h"
#include "key.h"
#include "lcd.h"
#include <PlayHistory.hmm>

#define HISTORY_BUFFER_SIZE 256
#define HISTORY_NAME_SIZE 11
#define HISTORY_VISIBLE 5

#ifndef CMMDEF
typedef struct history_item_t history_item_t;
typedef struct history_t history_t;
#endif

struct history_item_t
{
    SPTR(history_item_t, prev);
    SPTR(history_item_t, next);
    char PTR(path);
    unsigned int count;
};

struct history_t
{
    SPTR(history_item_t, front);
    SPTR(history_item_t, back);
    unsigned int count;
};

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

static void history_move_front(SPTR(history_t, history), SPTR(history_item_t, item))
{
    if (history == NULL || item == NULL || MEMBER(history, front) == item)
        return;

    if (MEMBER(item, prev) != NULL)
        MEMBER(MEMBER(item, prev), next) = MEMBER(item, next);
    if (MEMBER(item, next) != NULL)
        MEMBER(MEMBER(item, next), prev) = MEMBER(item, prev);
    else
        MEMBER(history, back) = MEMBER(item, prev);

    MEMBER(item, prev) = NULL;
    MEMBER(item, next) = MEMBER(history, front);
    MEMBER(MEMBER(history, front), prev) = item;
    MEMBER(history, front) = item;
}

static unsigned int history_record(SPTR(history_t, history), const char PTR(path))
{
    if (history == NULL || path == NULL || path[0] == '\0')
        return 0;

    for (SPTR(history_item_t, item) = MEMBER(history, front);
            item != NULL; item = MEMBER(item, next))
    {
        if (COND(history_same_path(MEMBER(item, path), path)))
        {
            if (cmpu(MEMBER(item, count), 0xffff) < 0)
                INC(MEMBER(item, count));
            history_move_front(history, item);
            return 1;
        }
    }

    unsigned int length = strLen(path);
    SPTR(history_item_t, item) = malloc(sizeof(history_item_t) + length + 1);
    if (item == NULL)
        return 0;

    MEMBER(item, prev) = NULL;
    MEMBER(item, next) = MEMBER(history, front);
    MEMBER(item, path) = addp(item, sizeof(history_item_t));
    MEMBER(item, count) = 1;
    for (unsigned int i = 0; i <= length; INC(i))
        MEMBER(item, path)[i] = path[i];

    if (MEMBER(history, front) != NULL)
        MEMBER(MEMBER(history, front), prev) = item;
    else
        MEMBER(history, back) = item;
    MEMBER(history, front) = item;
    INC(MEMBER(history, count));
    return 1;
}

static SPTR(history_t, history_alloc(VOID))
{
    SPTR(history_t, history) = malloc(sizeof(history_t));
    if (history == NULL)
        return NULL;
    MEMBER(history, front) = NULL;
    MEMBER(history, back) = NULL;
    MEMBER(history, count) = 0;
    return history;
}

static void history_free(SPTR(history_t, history))
{
    if (history == NULL)
        return;
    SPTR(history_item_t, item) = MEMBER(history, front);
    while (item != NULL)
    {
        SPTR(history_item_t, next) = MEMBER(item, next);
        free(item);
        item = next;
    }
    free(history);
}

static int history_read(SPTR(history_t, history))
{
    char PTR(buf) = malloc(HISTORY_BUFFER_SIZE);
    if (buf == NULL)
        return -1;

    int fd = open(PLAY_HISTORY_FILE, 0);
    if (fd < 0)
    {
        free(buf);
        return 0;
    }

    int used = 0;
    int ret = 0;
    for (;;)
    {
        if (used >= HISTORY_BUFFER_SIZE - 1)
        {
            ret = -1;
            break;
        }

        int len = read(fd, addp(buf, used), HISTORY_BUFFER_SIZE - used - 1);
        if (len < 0)
        {
            ret = -1;
            break;
        }
        ADDA(used, len);
        buf[used] = '\0';

        int start = 0;
        int i = 0;
        for (; i < used; INC(i))
        {
            if (buf[i] == '\n')
            {
                int end = i;
                if (end > start && buf[end - 1] == '\r')
                    DEC(end);
                buf[end] = '\0';
                if (end > start && !COND(history_record(history, addp(buf, start))))
                {
                    ret = -1;
                    break;
                }
                start = i + 1;
            }
        }
        if (ret < 0)
            break;

        if (start > 0)
        {
            for (int j = start; j < used; INC(j))
                buf[j - start] = buf[j];
            SUBA(used, start);
        }

        if (len == 0)
        {
            if (used > 0)
            {
                if (buf[used - 1] == '\r')
                    DEC(used);
                buf[used] = '\0';
                if (used > 0 && !COND(history_record(history, buf)))
                    ret = -1;
            }
            break;
        }
    }

    if (close(fd) < 0)
        ret = -1;
    free(buf);
    return ret;
}

static SPTR(history_item_t, history_item_at(SPTR(history_t, history), unsigned int index))
{
    SPTR(history_item_t, item) = MEMBER(history, front);
    for (unsigned int i = 0; i < index && item != NULL; INC(i))
        item = MEMBER(item, next);
    return item;
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

static void history_draw(SPTR(history_t, history), unsigned int cursor)
{
    history_clear();
    lcd_draw_string(0, 0, "# HISTORY #");

    unsigned int count = MEMBER(history, count);
    if (count == 0)
    {
        lcd_draw_string(1, 2, "(empty)");
    }
    else
    {
        unsigned int start = 0;
        if (cursor >= HISTORY_VISIBLE)
            start = cursor - HISTORY_VISIBLE + 1;

        SPTR(history_item_t, item) = history_item_at(history, start);
        char PTR(name) = malloc(HISTORY_NAME_SIZE);
        if (name != NULL)
        {
            for (unsigned int row = 0; row < HISTORY_VISIBLE && item != NULL; INC(row))
            {
                if (start + row == cursor)
                    lcd_draw_string(0, row + 1, "*");
                else
                    lcd_draw_string(0, row + 1, " ");
                history_short_name(MEMBER(item, path), name);
                lcd_draw_string(1, row + 1, name);
                lcd_draw_string(12, row + 1, "x");
                unsigned int shown = MEMBER(item, count);
                if (shown > 999)
                    shown = 999;
                lcd_draw_dec(13, row + 1, shown, 3);
                item = MEMBER(item, next);
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
    SPTR(history_t, history) = history_alloc();
    if (history == NULL)
        return;

    if (history_read(history) < 0)
    {
        history_clear();
        lcd_draw_string(0, 0, "# HISTORY #");
        lcd_draw_string(1, 3, "Read error");
        lcd_draw_string(0, 7, "BACK:Home");
        history_wait_release();
        for (;;)
        {
            key_step();
            if (COND(key_pressed(KEY_BACK)) || COND(key_pressed(KEY_ENTER)))
                break;
            sleep(10);
        }
        history_free(history);
        return;
    }

    unsigned int cursor = 0;
    history_wait_release();
    history_draw(history, cursor);

    while (COND(1))
    {
        key_step();
        unsigned int count = MEMBER(history, count);
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
            SPTR(history_item_t, item) = history_item_at(history, cursor);
            if (item != NULL)
            {
                char PTR(path) = MEMBER(item, path);
                history_wait_release();
                mp3_play_path(path);
                history_wait_release();
                history_record(history, path);
                cursor = 0;
                history_draw(history, cursor);
            }
        }
        else if (COND(key_pressed(KEY_BACK)))
        {
            history_free(history);
            return;
        }
        sleep(10);
    }
}
