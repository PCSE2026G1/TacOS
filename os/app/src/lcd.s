.display
    dw 1

; void lcd_draw_string(unsigned int x, unsigned int y, const char* value)
_lcd_draw_string
    push fp
    ld fp, sp

    ; locateXY(x, y)
    ld g0, 6, fp
    push g0
    ld g0, 4, fp
    push g0
    call _locateXY
    add sp, #4

    ; putStr(value)
    ld g0, 8, fp
    push g0
    call _putStr
    add sp, #2

    pop fp
    ret

; void lcd_set_display(int display)
_lcd_set_display
    ld g0, 2, sp
    st g0, .display
    ret

; int lcd_get_display(void)
_lcd_get_display
    ld g0, .display
    ret
