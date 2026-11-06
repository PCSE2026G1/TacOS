; void lcd_draw_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
_lcd_draw_rect
    push g3
    push g4
    push g5
    push g6
    push g7
    push g8
    ld g3, 14, sp  ; lcd
    ld g4, 16, sp  ; x
    ld g5, 18, sp  ; y
    ld g6, 20, sp  ; width
    ld g7, 22, sp  ; height
    ld g8, 24, sp  ; value

    ld g0, #1
    push g8
    push g0
    push g6
    push g5
    push g4
    push g3
    call _lcd_fill_rect  ; lcd_fill_rect(lcd, x, y, width, 1, value)

    ld g0, g5  ; y
    add g0, g7  ; y + height
    sub g0, #1  ; y + height - 1
    st g0, 4, sp
    call _lcd_fill_rect  ; lcd_fill_rect(lcd, x, y + height - 1, width, 1, value)

    ld g0, g5  ; y
    add g0, #1  ; y + 1
    st g0, 4, sp
    ld g0, #1
    st g0, 6, sp
    ld g0, g7  ; height
    sub g0, #2  ; height - 2
    st g0, 8, sp
    call _lcd_fill_rect  ; lcd_fill_rect(lcd, x, y + 1, 1, height - 2, value)

    ld g0, g4  ; x
    add g0, g6  ; x + width
    sub g0, #1  ; x + width - 1
    st g0, 2, sp
    call _lcd_fill_rect  ; lcd_fill_rect(lcd, x + width - 1, y + 1, 1, height - 2, value)

    add sp, #12
    pop g8
    pop g7
    pop g6
    pop g5
    pop g4
    pop g3
    ret
