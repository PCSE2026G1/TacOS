#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "cmmdef.h"

#ifndef CMMDEF
typedef struct playlist_item_t playlist_item_t;
typedef struct playlist_t playlist_t;
#endif

struct playlist_item_t
{
    SPTR(playlist_item_t, prev);
    SPTR(playlist_item_t, next);
    char PTR(name);
};

struct playlist_t
{
    SPTR(playlist_item_t, front);
    SPTR(playlist_item_t, back);
    unsigned int count;
};

extern SPTR(playlist_t, pl_alloc(VOID));
extern void pl_free(SPTR(playlist_t, playlist));

extern unsigned int pl_add(SPTR(playlist_t, playlist), const char PTR(name));
extern unsigned int pl_remove(SPTR(playlist_t, playlist), SPTR(playlist_item_t, item));

extern int pl_read(SPTR(playlist_t, playlist), const char PTR(name));
extern int pl_write(SPTR(const playlist_t, playlist), const char PTR(name));
extern int pl_append(const char PTR(playlist), const char PTR(name));

#endif
