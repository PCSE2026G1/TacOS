#ifndef HOME_H
#define HOME_H

/*
 * @startuml(id=home)
 * class home {
 *     +void home_show(void)
 * }
 *
 * home --> cmmdef
 * home --> mp3_player
 * home --> clock_viewer
 * home --> minesweeper
 * home --> mahjong
 * @enduml
 */

#include "cmmdef.h"

extern void home_show(VOID);

#endif
