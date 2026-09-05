#ifndef STACKSAFE_H
#define STACKSAFE_H

/*
 * @startuml(id=stacksafe)
 * class stacksafe {
 *     +int stacksafe(void* buf, unsigned int size, unsigned int func, unsigned int argc, ...)
 * }
 *
 * cmmdef ..> stacksafe
 * @enduml
 */

#include "cmmdef.h"

extern int stacksafe(void PTR(buf), unsigned int size, unsigned int func, unsigned int argc, ...);
#define STACKSAFE(buf, size, func, argc, ...) stacksafe(buf, size, INT_ADDR(func), argc, __VA_ARGS__)

#endif
