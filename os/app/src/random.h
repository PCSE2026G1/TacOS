#ifndef RANDOM_H
#define RANDOM_H

/*
 * @startuml(id=random)
 * class random {
 *     -unsigned long long seed
 *     +void llsrand(unsigned int seed)
 *     +int llrand(void)
 * }
 *
 * cmmdef ..> random
 * @enduml
 */

#include "cmmdef.h"

extern void llsrand(unsigned int seed);
extern int llrand(VOID);

#endif
