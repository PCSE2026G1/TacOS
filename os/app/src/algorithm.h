#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "cmmdef.h"

extern void swap(void PTR(l), void PTR(r), unsigned int size);
extern void shuffle(void PTR(ptr), unsigned int count, unsigned int size);
extern void hsort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void push_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void pop_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void isort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void insert(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));

#endif
