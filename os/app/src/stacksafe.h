#ifndef STACKSAFE_H
#define STACKSAFE_H

/*
 * @startuml(id=stacksafe)
 * class stacksafe {
 *     +int stacksafe(void* buf, unsigned int size, unsigned int func, unsigned int argc, ...)
 * }
 *
 * stacksafe ..> cmmdef
 * @enduml
 *
 * @startuml
 * () ABI as i1
 * () ABI as i2
 *
 * [Caller] - i1
 * i2 - [Callee]
 * [stacksafe] ..> i1
 * [stacksafe] ..> i2
 * @enduml
 *
 * @startuml
 * start
 * :スタックポインタを buf + size に設定;
 * if (argc) equals (0) then
 * else
 *     :フレームポインタを引数の最後に設定;
 *     repeat
 *         :フレームポインタから 2 を引く;
 *         :フレームポインタが指す値をスタックに追加する;
 *         :argc から 1 を引く;
 *     repeat while (argc が 0 と異なる)
 * endif
 * :func を呼び出す;
 * :フレームポインタを復元する;
 * :func の戻り値を返す;
 * stop
 * @enduml
 */

#include "cmmdef.h"

extern int stacksafe(void PTR(buf), unsigned int size, unsigned int func, unsigned int argc, ...);
#define STACKSAFE(buf, size, func, argc, ...) stacksafe(buf, size, INT_ADDR(func), argc, __VA_ARGS__)

#endif
