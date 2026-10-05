; void lcd_fill_rect(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int value)
_lcd_fill_rect
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
    push g9
    push g10

    push g8
    push g5
    push g4
    push g3
    ld g10, #0  ; i

.lcd_fill_rect_l0
    ld g0, g5  ; y
    add g0, g10  ; y + i
    st g0, 4, sp
    ld g9, #0  ; j

.lcd_fill_rect_l1
    ld g0, g4  ; x
    add g0, g9  ; x + j
    st g0, 2, sp
    call _lcd_draw

    add g9, #1  ; j++
    cmp g9, g6  ; j == width
    jnz .lcd_fill_rect_l1

    add g10, #1  ; i++
    cmp g10, g7  ; i == height
    jnz .lcd_fill_rect_l0

    add sp, #8
    pop g10
    pop g9
    pop g8
    pop g7
    pop g6
    pop g5
    pop g4
    pop g3
    ret
