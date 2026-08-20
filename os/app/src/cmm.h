#ifndef CMM_H
#define CMM_H

#include "cmmdef.h"

extern void PTR(malloc(unsigned int size));
extern void free(void PTR(ptr));

extern void locateXY(unsigned int x, unsigned int y);
extern void putStr(const char PTR(str));

extern void spiWriteLcdCom(const void PTR(buf));

#endif
