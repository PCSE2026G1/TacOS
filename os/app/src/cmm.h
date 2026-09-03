#ifndef CMM_H
#define CMM_H

#include "cmmdef.h"
#ifdef CLIB
#include <stdlib.h>
#endif

extern void dbgPutStr(const char PTR(str));
#ifdef CLIB
extern void clib_panic(const char PTR(msg), ...);
#define panic(msg, ...) clib_panic(msg, __VA_ARGS__)
#else
extern void panic(const char PTR(msg), ...);
#endif

extern unsigned int in(unsigned int p);
extern void out(unsigned int p, unsigned int v);

#ifdef CLIB
extern int clib_sleep(unsigned int ms);
#define sleep(ms) clib_sleep(ms)
#else
extern void PTR(malloc(unsigned int size));
extern void free(void PTR(ptr));
extern int sleep(unsigned int ms);
#endif

extern void locateXY(unsigned int x, unsigned int y);
extern void putStr(const char PTR(str));

extern void spiWriteLcdCom(const char PTR(buf));
extern void spiWriteLcdDat(const char PTR(buf), unsigned int len);

#endif
