#ifndef MAHJONG_H
#define MAHJONG_H

/*
 * @startuml(id=mahjong)
 * class mahjong {
 *     -int mj_comp_tiles(const void* l, const void* r)
 *     -void mj_init_tiles(char* tiles)
 *     -void mj_show_tiles(char* tiles)
 *     -void mj_count_tiles(const char* tiles, int tile, char* counts)
 *     -int mj_test_hand(const char* tiles, int tile, void* buf)
 *     -unsigned int mj_tenpai(const void* tiles, void* buf)
 *     -void mj_show_tenpai(unsigned int x, unsigned int y, unsigned int bits, void* buf)
 *     -void play(void* buf)
 *     +void mj_play(void)
 * }
 *
 * cmmdef ..> mahjong
 * tac_assert ..> mahjong
 * algorithm --> mahjong
 * tac_string --> mahjong
 * key --> mahjong
 * lcd --> mahjong
 * random --> mahjong
 * stacksafe ..> mahjong
 * @enduml
 *
 * @startuml
 * [*] --> Select
 * Select --> [*] : BACK ボタンを押す
 * Select --> Result : OK を選択
 * Result --> Select : 正解
 * Result --> [*] : 不正解
 *
 * Select : 聴牌を選択する.
 * Result : 正解を確認する.
 * @enduml
 */

#include "cmmdef.h"

extern void mj_play(VOID);

#endif
