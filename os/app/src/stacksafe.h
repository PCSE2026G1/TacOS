#ifndef STACKSAFE_H
#define STACKSAFE_H

#include "cmmdef.h"

extern int stacksafe(void PTR(buf), unsigned int size, unsigned int func, unsigned int argc, ...);
#ifdef CLIB
#define STACKSAFE(buf, size, func, argc, ...) ((func)(__VA_ARGS__))
#else
#define STACKSAFE(buf, size, func, argc, ...) stacksafe(buf, size, INT_ADDR(func), argc, __VA_ARGS__)
#endif

#endif
