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

; void lcd_set_display(int value)
_lcd_set_display
    push g3
    ld g3, 4, sp

    ; if (value) value = 1
    cmp g3, #0
    jz .lcd_set_display_l0
    ld g3, #1

.lcd_set_display_l0
    ; buf[1] = 0xff
    ld g0, #0xff
    push g0

    ; buf[0] = value | 0xae
    ld g0, g3
    or g0, #0xae
    push g0

    ; spiWriteLcdCom(buf)
    ld g0, sp
    push g0
    call _spiWriteLcdCom
    add sp, #6

    ; display = value
    st g3, .display

    pop g3
    ret

; int lcd_get_display(void)
_lcd_get_display
    ld g0, .display
    ret
