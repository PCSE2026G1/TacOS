#include "playlist.h"
#include "cmmdef.h"
#include "cmm.h"
#include "tac_string.h"

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
