#ifndef STACKSAFE_H
#define STACKSAFE_H

#include "cmmdef.h"

extern int stacksafe(void PTR(buf), unsigned int size, unsigned int func, unsigned int argc, ...);

#endif
