#include "algorithm.h"
#include "cmmdef.h"
#include "random.h"

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

extern void shuffle(void PTR(ptr), unsigned int count, unsigned int size)
{
    for (unsigned int i = 0; i < count; INC(i))
    {
        unsigned int j = mulul(llrand() << 1, count - i) + i;
        swap(addp(ptr, i * size), addp(ptr, j * size), size);
    }
}

extern void hsort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*))
{
    for (unsigned int i = 1; i <= count; INC(i))
        push_heap(ptr, i, size, comp);
    for (unsigned int i = 0; i < count; INC(i))
        pop_heap(ptr, count - i, size, comp);
}

extern void push_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*))
{
    for (unsigned int i = count, j = i >> 1; i > 1; i = j, j = i >> 1)
    {
        void PTR(p1) = addp(ptr, (i - 1) * size);
        void PTR(p2) = addp(ptr, (j - 1) * size);
        if (CALL(comp, 2, p1, p2) <= 0)
            break;
        swap(p1, p2, size);
    }
}

extern void pop_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*))
{
    if (count >= 2)
    {
        swap(ptr, addp(ptr, (count - 1) * size), size);
        for (unsigned int i = 1, j = i << 1; j < count; i = j, j = i << 1)
        {
            void PTR(p1) = addp(ptr, (i - 1) * size);
            void PTR(p2) = addp(ptr, (j - 1) * size);
            if (j + 1 < count)
            {
                void PTR(p3) = addp(ptr, j * size);
                if (CALL(comp, 2, p2, p3) < 0)
                {
                    INC(j);
                    p2 = p3;
                }
            }
            if (CALL(comp, 2, p1, p2) >= 0)
                break;
            swap(p1, p2, size);
        }
    }
}

extern void isort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*))
{
    for (unsigned int i = 1; i <= count; INC(i))
        insert(ptr, i, size, comp);
}

extern void insert(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*))
{
    for (unsigned int i = count - 1; i > 0; DEC(i))
    {
        void PTR(p1) = addp(ptr, i * size);
        void PTR(p2) = addp(ptr, (i - 1) * size);
        if (CALL(comp, 2, p1, p2) >= 0)
            break;
        swap(p1, p2, size);
    }
}
