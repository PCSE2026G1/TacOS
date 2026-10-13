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
 * random ..> cmmdef
 * @enduml
 *
 * @startuml
 * start
 * :seed のポインタに 8 を足す;
 * :キャリーの初期値を 31 とする;
 * repeat
 *     :seed のポインタから 1 を引く;
 *     :seed のポインタが指すバイトに 17 を掛けキャリーを足す;
 *     :seed のポインタが指すバイトに演算結果を代入する;
 *     :演算結果の上位 8 ビットを次のキャリーとする;
 * repeat while (seed のポインタが初期値と異なる)
 * :seed の上位 15 ビットを返す;
 * stop
 * @enduml
 */

#include "cmmdef.h"

extern void llsrand(unsigned int seed);
extern int llrand(VOID);

#endif
