#include "string.h"
#include "cmmdef.h"
#include "assert.h"

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
