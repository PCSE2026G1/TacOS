#ifndef DEBUG_H
#define DEBUG_H

#include "cmmdef.h"

extern unsigned int get_sp(VOID);
extern void puts(const char PTR(str));
extern void puti(int num);
extern void putx(unsigned int num);

#endif
