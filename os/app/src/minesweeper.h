#ifndef MINESWEEPER_H
#define MINESWEEPER_H

/*
 * @startuml(id=minesweeper)
 * class minesweeper {
 *     -void* ms_ptr(const void* buf, unsigned int x, unsigned int y)
 *     -unsigned int ms_count(const void* buf, unsigned int x, unsigned int y, unsigned int mask, unsigned int value)
 *     -void ms_init(void* buf, unsigned int x, unsigned int y)
 *     -unsigned int ms_open(void* buf, unsigned int x, unsignd int y)
 *     -unsigned int ms_x(unsigned int j)
 *     -unsigned int ms_y(unsigned int i)
 *     -void ms_draw(lcd_t* lcd, const void* buf, unsigned int cx, unsigned int cy, unsigned int rem)
 *     -void play(void* buf)
 *     ~void ms_draw_icon(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int num)
 *     +void ms_play(void)
 * }
 *
 * enum MS {
 *     ~MS_MINE_MASK
 *     ~MS_NUM_MASK
 *     ~MS_OPENED_MASK
 *     ~MS_FLAGGED_MASK
 * }
 *
 * minesweeper *-- MS
 * cmmdef ..> minesweeper
 * tac_string --> minesweeper
 * random --> minesweeper
 * algorithm --> minesweeper
 * lcd --> minesweeper
 * key --> minesweeper
 * stacksafe ..> minesweeper
 * @enduml
 *
 * @startuml(id=minesweeper_states)
 * [*] --> Start
 * Start --> [*] : キャンセル
 * Start --> Play : マスを選択
 * Play --> End : クリア, または地雷を選択
 * End --> [*] : 決定
 *
 * Start : 最初のマスを選択する.
 * Start : BACK ボタンでキャンセルする.
 *
 * Play : マスを選択する.
 *
 * End : 地雷の場所を確認する.
 * @enduml
 */

#include "cmmdef.h"

extern void ms_play(VOID);

#endif
