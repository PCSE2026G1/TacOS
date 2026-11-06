#include "tac_string.h"
#include "cmmdef.h"
#include "tac_assert.h"

extern void PTR(memcpy(void PTR(dest), const void PTR(src), unsigned int count))
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(cmpp(dest, src) <= 0 || cmpp(dest, addp(src, count)) >= 0);
    for (unsigned int i = 0; i != count; INC(i))
        setp(addp(dest, i), getp(addp(src, i)));
    return dest;
}

extern void PTR(memmovr(void PTR(dest), const void PTR(src), unsigned int count))
{
    assert(dest != NULL);
    assert(src != NULL);
    assert(cmpp(dest, src) >= 0 || cmpp(addp(dest, count), src) <= 0);
    for (unsigned int i = count - 1; i != 0xffff; DEC(i))
        setp(addp(dest, i), getp(addp(src, i)));
    return dest;
}

extern void PTR(memmove(void PTR(dest), const void PTR(src), unsigned int count))
{
    int cmp = cmpp(dest, src);
    if (cmp < 0)
        return memcpy(dest, src, count);
    if (cmp > 0)
        return memmovr(dest, src, count);
    return dest;
}

extern void PTR(memset(void PTR(dest), int ch, unsigned int count))
{
    assert(dest != NULL);
    for (unsigned int i = 0; i != count; INC(i))
    {
        setp(addp(dest, i), ch);
    }
    return dest;
}

extern char PTR(itoa(char PTR(buf), unsigned int size, int num))
{
    assert(buf != NULL);
    assert(size >= 1);
    unsigned int i = size - 1;
    buf[i] = '\0';
    int sign = 0;
    if (num < 0)
    {
        sign = 1;
        num = -num;
    }

    do
    {
        if (i == 0)
            return NULL;
        DEC(i);
        int ch = 0x30 + (num % 10);
        setp(addp(buf, i), ch);
        DIVA(num, 10);
    } while (COND(num));

    if (COND(sign))
    {
        if (i == 0)
            return NULL;
        DEC(i);
        buf[i] = '-';
    }

    return addp(buf, i);
}

extern char PTR(itox(char PTR(buf), unsigned int size, unsigned int num))
{
    assert(buf != NULL);
    assert(size >= 1);
    unsigned int i = size - 1;
    buf[i] = '\0';

    do
    {
        if (i == 0)
            return NULL;
        DEC(i);
        int ch = num & 15;
        if (ch < 10)
            ADDA(ch, 0x30);
        else
            ADDA(ch, 0x41 - 10);
        setp(addp(buf, i), ch);
        num = (num >> 4) & 0x0fff;
    } while (COND(num));

    if (i > 0)
        memset(buf, '0', i);
    return buf;
}
