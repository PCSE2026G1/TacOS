#include "cmm.h"
#include "playlist.h"
#include "debug.h"

static void pl_print(SPTR(const playlist_t, playlist))
{
    dbgPutStr("[");
    int comma = 0;
    for (SPTR(const playlist_item_t, item) = MEMBER(playlist, front); item != NULL; item = item->next)
    {
        if (COND(comma))
        {
            dbgPutStr(", ");
        }
        else
        {
            comma = 1;
        }
        dbgPutStr(item->name);
    }
    dbgPutStr("], ");
    puti(playlist->count);
}

void pl_test(VOID)
{
    SPTR(playlist_t, l) = malloc(sizeof(playlist_t));
    pl_print(l);
    pl_add(l, "foo");
    pl_add(l, "bar");
    pl_add(l, "baz");
    pl_add(l, "qux");
    pl_add(l, "quux");
    pl_print(l);
    pl_write(l, "/test.txt");
    pl_remove(l, l->back);
    pl_print(l);
    pl_remove(l, l->front);
    pl_print(l);
    pl_remove(l, l->front->next);
    pl_print(l);
    pl_remove(l, l->back);
    pl_print(l);
    pl_remove(l, l->back);
    pl_print(l);
    pl_read(l, "/test.txt");
    pl_print(l);
    free(l);
}
