#ifndef MINESWEEPER0_H
#define MINESWEEPER0_H

#include "cmmdef.h"
#include "lcd.h"

#ifndef MS_COUNT
#define MS_COUNT 16
#endif

#define MS_MINE_MASK 0x0a
#define MS_NUM_MASK 0x0f
#define MS_OPENED_MASK 0x10
#define MS_FLAGGED_MASK 0x20

extern void ms_draw_icon(STRUCT(lcd_t) PTR(lcd),
        unsigned int x, unsigned int y, unsigned int num);

#endif
