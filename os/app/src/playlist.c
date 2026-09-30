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
    unsigned int str_size = strLen(name);
    SPTR(playlist_item_t, item) = malloc(sizeof(playlist_item_t) + str_size + 1);
    if (item == NULL)
        return 0;
    if (MEMBER(playlist, back) == NULL)
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
    return 0;
}

extern int pl_read(SPTR(playlist_t, playlist), const char PTR(name))
{
    char PTR(buf) = malloc(PL_BUFFER_SIZE);
    if (buf == NULL)
        return -1;
    int fd = open(name, 0);
    if (fd < 0)
    {
        free(buf);
        return -1;
    }
    void PTR(p) = buf;
    int len;
    while ((len = read(fd, p, PL_BUFFER_SIZE - subpp(p, buf))) >= 0)
    {
        p = addp(p, len);
        len = subpp(p, buf);
        if (len == 0)
            break;
        if (buf[0] == '\n')
        {
            memmove(buf, addp(buf, 1), len - 1);
            p = subp(p, 1);
            continue;
        }
        int i = 0;
        for (; i < len && buf[i] != '\n'; INC(i));
        assert(i < PL_BUFFER_SIZE);
        buf[i] = '\0';
        pl_add(playlist, buf);
        memmove(buf, addp(buf, i + 1), len - i - 1);
        p = subp(p, i + 1);
    }
    if (close(fd) == -1)
        len = -1;
    free(buf);
    if (len == -1)
        return -1;
    return 0;
}

extern int pl_write(SPTR(const playlist_t, playlist), const char PTR(name))
{
    int fd = open(name, 1);
    if (fd < 0)
        return -1;
    int ret = 0;
    for (SPTR(const playlist_item_t, item) = MEMBER(playlist, front); item != NULL; item = MEMBER(item, next))
    {
        if (write(fd, MEMBER(item, name), strLen(MEMBER(item, name))) < 0 || write(fd, "\n", 1) < 0)
            ret = -1;
    }
    if (close(fd) < 0)
        ret = -1;
    return ret;
}

extern int pl_append(const char PTR(playlist), const char PTR(name))
{
    int fd = open(playlist, 2);
    if (fd < -1)
        return -1;
    int ret = 0;
    if (write(fd, name, strLen(name)) < 0 || write(fd, "\n", 1) < 0)
        ret = -1;
    if (close(fd) < 0)
        ret = -1;
    return ret;
}
