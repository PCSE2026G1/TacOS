#ifndef DEBUG_H
#define DEBUG_H

/*
 * @startuml(id=debug)
 * class debug {
 *     +void puts(const char* str)
 *     +void puti(int num)
 *     +void putx(unsigned int num)
 * }
 *
 * cmmdef ..> debug
 * tac_assert ..> debug
 * tac_string --> debug
 * @enduml
 */

#include "cmmdef.h"

extern unsigned int get_sp(VOID);
extern void puts(const char PTR(str));
extern void puti(int num);
extern void putx(unsigned int num);

#endif
