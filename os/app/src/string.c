#include "string.h"
#include "cmmdef.h"

extern void PTR(memset(void PTR(dest), int ch, unsigned int count))
{
    for (unsigned int i = 0; i != count; INC(i))
    {
        setp(addp(dest, i), ch);
    }
    return dest;
}
