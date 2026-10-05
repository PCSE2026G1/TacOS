#include "playlist.h"
#include "cmmdef.h"
#include "cmm.h"
#include "key.h"

#define PLAYLIST_FILE "/PLAYLIST.TXT"
#define PLAYLIST_VISIBLE 5

static SPTR(playlist_item_t, playlist_item_at(SPTR(playlist_t, playlist), unsigned int index))
{
    SPTR(playlist_item_t, item) = MEMBER(playlist, front);
    for (unsigned int i = 0; i < index && item != NULL; INC(i))
        item = MEMBER(item, next);
    return item;
}

static const char PTR(playlist_short_name(const char PTR(path)))
{
    const char PTR(name) = path;
    for (unsigned int i = 0; path[i] != '\0'; INC(i))
        if (path[i] == '/')
            name = addp(path, i + 1);
    return name;
}

static void playlist_clear(VOID)
{
    for (unsigned int y = 0; y < 8; INC(y))
        lcd_draw_string(0, y, "                ");
}

static void playlist_draw(SPTR(playlist_t, playlist), unsigned int cursor)
{
    playlist_clear();
    lcd_draw_string(0, 0, "# PLAYLIST #");

    unsigned int count = MEMBER(playlist, count);
    if (count == 0)
    {
        lcd_draw_string(1, 2, "(empty)");
    }
    else
    {
        unsigned int start = 0;
        if (cursor >= PLAYLIST_VISIBLE)
            start = cursor - PLAYLIST_VISIBLE + 1;

        SPTR(playlist_item_t, item) = playlist_item_at(playlist, start);
        for (unsigned int row = 0; row < PLAYLIST_VISIBLE && item != NULL; INC(row))
        {
            if (start + row == cursor)
                lcd_draw_string(0, row + 1, "*");
            else
                lcd_draw_string(0, row + 1, " ");
            lcd_draw_string(2, row + 1, playlist_short_name(MEMBER(item, name)));
            item = MEMBER(item, next);
        }
    }

    lcd_draw_string(0, 6, "LT:Add RT:Delete");
    lcd_draw_string(0, 7, "ENT:Play BACK");
}

static void playlist_wait_release(VOID)
{
    while (COND(key_read()))
        sleep(10);
    key_step();
    key_step();
}

extern void playlist_show(VOID)
{
    SPTR(playlist_t, playlist) = pl_alloc();
    if (playlist == NULL)
        return;

    if (pl_read(playlist, PLAYLIST_FILE) < 0)
    {
        if (pl_write(playlist, PLAYLIST_FILE) < 0)
        {
            pl_free(playlist);
            return;
        }
    }

    unsigned int cursor = 0;
    playlist_wait_release();
    playlist_draw(playlist, cursor);

    while (COND(1))
    {
        key_step();
        unsigned int count = MEMBER(playlist, count);

        if (COND(key_pressed(KEY_UP)) && cursor > 0)
        {
            DEC(cursor);
            playlist_draw(playlist, cursor);
        }
        else if (COND(key_pressed(KEY_DOWN)) && cursor + 1 < count)
        {
            INC(cursor);
            playlist_draw(playlist, cursor);
        }
        else if (COND(key_pressed(KEY_LEFT)))
        {
            char PTR(path) = malloc(256);
            if (path != NULL)
            {
                path[0] = '\0';
                chooserMp3(path);
                if (path[0] != '\0' && pl_add(playlist, path) != 0)
                {
                    pl_write(playlist, PLAYLIST_FILE);
                    cursor = MEMBER(playlist, count) - 1;
                }
                free(path);
                playlist_wait_release();
                playlist_draw(playlist, cursor);
            }
        }
        else if (COND(key_pressed(KEY_RIGHT)) && count > 0)
        {
            SPTR(playlist_item_t, item) = playlist_item_at(playlist, cursor);
            if (item != NULL && pl_remove(playlist, item) != 0)
            {
                pl_write(playlist, PLAYLIST_FILE);
                count = MEMBER(playlist, count);
                if (count == 0)
                    cursor = 0;
                else if (cursor >= count)
                    cursor = count - 1;
                playlist_draw(playlist, cursor);
            }
        }
        else if (COND(key_pressed(KEY_ENTER)) && count > 0)
        {
            char PTR(PTR(paths)) = malloc(count * 2);
            if (paths != NULL)
            {
                pl_to_array(playlist, paths);
                playlist_wait_release();
                mp3_play_playlist(paths, count, cursor);
                playlist_wait_release();
                free(paths);
                playlist_draw(playlist, cursor);
            }
        }
        else if (COND(key_pressed(KEY_BACK)))
        {
            pl_free(playlist);
            return;
        }

        sleep(10);
    }
}
