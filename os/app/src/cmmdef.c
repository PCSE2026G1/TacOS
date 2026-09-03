#ifdef CMMDEF
#error "Not C--"
#endif
#include "cmmdef.h"

void* addp(const void* l, int r)
{
    return (char*)l + r;
}

void* subp(const void* l, int r)
{
    return (char*)l - r;
}

int subpp(const void* l, const void* r)
{
    return (const char*)l - (const char*)r;
}

unsigned int getp(const void* p)
{
    return *(const unsigned char*)p;
}

void setp(void* p, int value)
{
    *(unsigned char*)p = value;
}

void* addp2(const void* l, int r)
{
    return addp(l, r * 2);
}

void* subp2(const void* l, int r)
{
    return subp(l, r * 2);
}

int subpp2(const void* l, const void* r)
{
    return subpp(l, r) / 2;
}

unsigned int getp2(const void* p)
{
    return *(unsigned short int*)p;
}

void setp2(void* p, unsigned int value)
{
    *(unsigned short int*)p = value;
}

int cmpu(unsigned int l, unsigned int r)
{
    if (l < r)
        return -1;
    if (l > r)
        return 1;
    return 0;
}

int cmpp(const void* l, const void* r)
{
    if ((const char*)l < (const char*)r)
        return -1;
    if ((const char*)l > (const char*)r)
        return 1;
    return 0;
}

unsigned int cond3(int cond, unsigned int l, unsigned int r)
{
    return cond ? l : r;
}
