#ifndef TAC_STRING_H
#define TAC_STRING_H

/*
 * @startuml(id=tac_string)
 * class tac_string {
 *     +void* memcpy(void* dest, const void* src, unsigned int count)
 *     +void* memmove(void* dest, const void* src, unsigned int count)
 *     +void* memovr(void* dest, const void* src, unsigned int count)
 *     +void* memset(void* dest, int ch, unsigned int count)
 *     +char* itoa(char* buf, unsigned int size, int num)
 *     +char* itox(char* buf, unsigned int size, unsigned int num)
 * }
 *
 * cmmdef ..> tac_string
 * tac_assert ..> tac_string
 * @enduml
 */

#include "cmmdef.h"

extern void PTR(memcpy(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memmove(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memmovr(void PTR(dest), const void PTR(src), unsigned int count));
extern void PTR(memset(void PTR(dest), int ch, unsigned int count));

extern char PTR(itoa(char PTR(buf), unsigned int size, int num));
extern char PTR(itox(char PTR(buf), unsigned int size, unsigned int num));

#endif
