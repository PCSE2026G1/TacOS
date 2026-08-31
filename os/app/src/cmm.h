#ifndef CMM_H
#define CMM_H

#include "cmmdef.h"

extern void dbgPutStr(const char PTR(str));
extern void panic(const char PTR(msg), ...);

extern unsigned int in(unsigned int p);
extern void out(unsigned int p, unsigned int v);

extern void PTR(malloc(unsigned int size));
extern void free(void PTR(ptr));
extern void sleep(unsigned int ms);

extern void locateXY(unsigned int x, unsigned int y);
extern void putStr(const char PTR(str));

extern void spiWriteLcdCom(const char PTR(buf));
extern void spiWriteLcdDat(const char PTR(buf), unsigned int len);

#endif
