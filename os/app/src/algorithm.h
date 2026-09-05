#ifndef ALGORITHM_H
#define ALGORITHM_H

/*
 * @startuml(id=algorithm)
 * class algorithm {
 *     +void swap(void* l, void* r, unsigned int size)
 *     +void shuffle(void* ptr, unsigned int count, unsigned int size)
 *     +void hsort(void* ptr, unsigned int count, unsigned int size, int (*comp)(const void*, const void*))
 *     +void push_heap(void* ptr, unsigned int count, unsigned int size, int (*comp)(const void*, const void*))
 *     +void pop_heap(void* ptr, unsigned int count, unsigned int size, int (*comp)(const void*, const void*))
 *     +void isort(void* ptr, unsigned int count, unsigned int size, int (*comp)(const void*, const void*))
 *     +void insert(void* ptr, unsigned int count, unsigned int size, int (*comp)(const void*, const void*))
 * }
 *
 * cmmdef ..> algorithm
 * random --> algorithm
 * @enduml
 */

#include "cmmdef.h"

extern void swap(void PTR(l), void PTR(r), unsigned int size);
extern void shuffle(void PTR(ptr), unsigned int count, unsigned int size);
extern void hsort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void push_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void pop_heap(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void isort(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));
extern void insert(void PTR(ptr), unsigned int count, unsigned int size, FPTR(int, comp, const void*, const void*));

#endif
