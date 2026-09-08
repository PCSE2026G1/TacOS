.display dw 1

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

; void lcd_draw_dec(unsigned int x, unsigned int y, unsigned int num, unsigned int width)
_lcd_draw_dec
    push fp
    ld fp, sp
    push g3
    push g4

    ld g3, 8, fp  ; num
    ld g4, 4, fp  ; x
    add g4, 10, fp  ; x += width

    sub sp, #1
    ld g0, #0
    st g0, @sp

.lcd_draw_dec_l0
    ld g0, g3  ; d = num
    div g3, #10  ; num /= 10
    ld g1, g3  ; num
    mul g1, #10  ; num * 10
    sub g0, g1  ; d %= 10
    add g0, #48  ; d += 0x30
    sub sp, #1
    st g0, @sp

    sub g4, #1  ; x--
    cmp g3, #0  ; num == 0
    jnz .lcd_draw_dec_l0

    ld g0, sp
    and sp, #0xfffe
    push g0
    ld g0, 6, fp
    push g0
    push g4
    call _lcd_draw_string
    ld sp, fp
    sub sp, #4

    pop g4
    pop g3
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
