#include "key.h"
#include "cmmdef.h"
#include "cmm.h"
#include "tac_assert.h"

static unsigned int key0 = 0;
static unsigned int key1 = 0;
static unsigned int key2 = 0;

extern unsigned int key_read(VOID)
{
    return ~in(0x18) & KEY_ALL_MASK;
}

extern void key_step(VOID)
{
    unsigned int key = key_read();
    key1 = key2;
    key2 = (key2 | (key & key0)) & (key | key0);
    key0 = key;
}

extern unsigned int key_get(VOID)
{
    return key2;
}

extern int key_pressed(unsigned int key)
{
    assert(cmpu(key, 6) < 0);
    return (~key1 & key2) >> key & 1;
}

extern int key_pressed_mask(unsigned int mask)
{
    assert(!COND(mask & ~KEY_ALL_MASK));
    if ((~key1 & key2 & mask) == mask)
        return 1;
    return 0;
}
