#include "playlist.h"
#include "cmmdef.h"
#include "cmm.h"
#include "tac_assert.h"
#include "tac_string.h"

#ifndef PL_BUFFER_SIZE
#define PL_BUFFER_SIZE 128
#endif

extern SPTR(playlist_t, pl_alloc(VOID))
{
    SPTR(playlist_t, playlist) = malloc(sizeof(playlist_t));
    if (playlist == NULL)
        return NULL;
    MEMBER(playlist, front) = NULL;
    MEMBER(playlist, back) = NULL;
    MEMBER(playlist, count) = 0;
    return playlist;
}

extern void pl_free(SPTR(playlist_t, playlist))
{
    if (playlist == NULL)
        return;
    SPTR(playlist_item_t, item) = MEMBER(playlist, front);
    while (item != NULL)
    {
        SPTR(playlist_item_t, next) = MEMBER(item, next);
        free(item);
        item = next;
    }
    free(playlist);
}

extern unsigned int pl_add(SPTR(playlist_t, playlist), const char PTR(name))
{
    if (playlist == NULL || name == NULL)
        return 0;
    unsigned int str_size = strLen(name);
    SPTR(playlist_item_t, item) = malloc(sizeof(playlist_item_t) + str_size + 1);
    if (item == NULL)
        return 0;
    if (playlist == NULL || item == NULL || MEMBER(playlist, back) == NULL)
    {
        MEMBER(playlist, front) = item;
    }
    else
    {
        MEMBER(MEMBER(playlist, back), next) = item;
    }
    MEMBER(item, prev) = MEMBER(playlist, back);
    MEMBER(item, next) = NULL;
    MEMBER(item, name) = addp(item, sizeof(playlist_item_t));
    memcpy(MEMBER(item, name), name, str_size + 1);
    MEMBER(playlist, back) = item;
    INC(MEMBER(playlist, count));
    return 1;
}

extern unsigned int pl_remove(SPTR(playlist_t, playlist), SPTR(playlist_item_t, item))
{
    if (MEMBER(playlist, back) == NULL)
        return 0;
    if (MEMBER(playlist, front) == item)
    {
        MEMBER(playlist, front) = MEMBER(item, next);
    }
    else
    {
        MEMBER(MEMBER(item, prev), next) = MEMBER(item, next);
    }
    if (MEMBER(playlist, back) == item)
    {
        MEMBER(playlist, back) = MEMBER(item, prev);
    }
    else
    {
        MEMBER(MEMBER(item, next), prev) = MEMBER(item, prev);
    }
    free(item);
    DEC(MEMBER(playlist, count));
    return 1;
}

extern int pl_read(SPTR(playlist_t, playlist), const char PTR(name))
{
    if (playlist == NULL || name == NULL)
        return -1;
    char PTR(buf) = malloc(PL_BUFFER_SIZE);
    if (buf == NULL)
        return -1;
    int fd = open(name, 0);
    if (fd < 0)
    {
        free(buf);
        return -1;
    }
    int used = 0;
    int len = 0;
    int ret = 0;
    for (;;)
    {
        if (used >= PL_BUFFER_SIZE - 1)
        {
            ret = -1;
            break;
        }
        len = read(fd, addp(buf, used), PL_BUFFER_SIZE - used - 1);
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
                if (end > start && pl_add(playlist, addp(buf, start)) == 0)
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
            memmove(buf, addp(buf, start), used - start);
            SUBA(used, start);
        }
        if (len == 0)
        {
            if (used > 0)
            {
                if (buf[used - 1] == '\r')
                    DEC(used);
                buf[used] = '\0';
                if (used > 0 && pl_add(playlist, buf) == 0)
                    ret = -1;
            }
            break;
        }
    }
    if (close(fd) == -1)
        ret = -1;
    free(buf);
    return ret;
}

extern int pl_write(SPTR(const playlist_t, playlist), const char PTR(name))
{
    if (playlist == NULL || name == NULL)
        return -1;
    remove(name);
    if (creat(name) < 0)
        return -1;
    int fd = open(name, 1);
    if (fd < 0)
        return -1;
    int ret = 0;
    for (SPTR(const playlist_item_t, item) = MEMBER(playlist, front); item != NULL; item = MEMBER(item, next))
    {
        unsigned int len = strLen(MEMBER(item, name));
        if (write(fd, MEMBER(item, name), len) != len || write(fd, "\n", 1) != 1)
            ret = -1;
    }
    if (close(fd) < 0)
        ret = -1;
    return ret;
}

extern int pl_append(const char PTR(playlist), const char PTR(name))
{
    if (playlist == NULL || name == NULL)
        return -1;

    SPTR(playlist_t, list) = pl_alloc();
    if (list == NULL)
        return -1;

    int fd = open(playlist, 0);
    if (fd >= 0)
    {
        if (close(fd) < 0 || pl_read(list, playlist) < 0)
        {
            pl_free(list);
            return -1;
        }
    }

    if (pl_add(list, name) == 0)
    {
        pl_free(list);
        return -1;
    }

    int ret = pl_write(list, playlist);
    pl_free(list);
    return ret;
}

extern unsigned int pl_to_array(SPTR(const playlist_t, playlist), const char PTR(PTR(dest)))
{
    unsigned int count = 0;
    for (SPTR(const playlist_item_t, item) = MEMBER(playlist, front); item != NULL; item = MEMBER(item, next))
    {
        dest[count] = MEMBER(item, name);
        INC(count);
    }
    assert(count == MEMBER(playlist, count));
    return count;
}

extern unsigned int pl_to_names(SPTR(const playlist_t, playlist), const char PTR(PTR(dest)))
{
    unsigned int count = pl_to_array(playlist, dest);
    for (unsigned int i = 0; i < count; INC(i))
    {
        unsigned int j = 0;
        for (unsigned int k = 0; dest[i][k] != '\0'; INC(k))
            if (dest[i][k] == '/')
                j = k + 1;
        dest[i] = addp(dest[i], j);
    }
    return count;
}
