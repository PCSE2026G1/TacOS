#include "algorithm.h"

extern void swap(void PTR(l), void PTR(r), unsigned int size)
{
    for (unsigned int i = 0; i != size; INC(i), l = addp(l, 1), r = addp(r, 1))
    {
        unsigned int a = getp(l);
        unsigned int b = getp(r);
        setp(l, b);
        setp(r, a);
    }
}
