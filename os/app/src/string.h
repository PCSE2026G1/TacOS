#ifndef STRING_H
#define STRING_H

#include "cmmdef.h"

extern void PTR(memcpy(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memmove(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memmovr(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memset(void PTR(dest), int ch, unsigned int count));

extern char PTR(itoa(char PTR(buf), unsigned int size, int num));
extern char PTR(itox(char PTR(buf), unsigned int size, unsigned int num));

#endif
