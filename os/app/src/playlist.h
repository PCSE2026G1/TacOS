#ifndef PLAYLIST_H
#define PLAYLIST_H

/*
 * @startuml(id=playlist)
 * class playlist {
 *     +playlist_t* pl_alloc(void)
 *     +void pl_free(playlist_t* playlist)
 *     +unsigned int pl_add(playlist_t* playlist, const char* name)
 *     +unsigned int pl_remove(playlist_t* playlist, playlist_item_t* item)
 *     +int pl_read(playlist_t* playlist, const char* name)
 *     +int pl_write(const playlist_t* playlist, const char* name)
 *     +int pl_append(const char* playlist, const char* name)
 *     +unsigned int pl_to_array(const playlist_t* playlist, const char** dest)
 *     +unsigned int pl_to_names(const playlist_t* playlist, const char** dest)
 * }
 *
 * struct playlist_item_t {
 *     +playlist_item_t* prev
 *     +playlist_item_t* next
 *     +char* name
 * }
 *
 * struct playlist_t {
 *     +playlist_item_t* front
 *     +playlist_item_t* back
 *     +unsigned int count
 * }
 *
 * playlist *-- playlist_t
 * playlist_t *-- playlist_item_t
 * playlist --> cmmdef
 * playlist --> tac_string
 * @enduml
 */

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

extern unsigned int pl_to_array(SPTR(const playlist_t, playlist), const char PTR(PTR(dest)));
extern unsigned int pl_to_names(SPTR(const playlist_t, playlist), const char PTR(PTR(dest)));

#endif
