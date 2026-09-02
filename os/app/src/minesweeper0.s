; static unsigned char icons[48]
.icons string "\x75\x55\x70"
.icon1 string "\x22\x22\x20"
.icon2 string "\x71\x74\x70"
.icon3 string "\x71\x71\x70"
.icon4 string "\x55\x71\x10"
.icon5 string "\x74\x71\x70"
.icon6 string "\x74\x75\x70"
.icon7 string "\x71\x11\x10"
.icon8 string "\x75\x75\x70"
.icon9 string "\x75\x71\x70"
.icon10 string "\x27\xd7\x20"
.icon11 string "\x77\x44\x40"

; void ms_draw_icon(lcd_t* lcd, unsigned int x, unsigned int y, unsigned int num)
_ms_draw_icon
    push g8
    push g9
    push g10
    push g11

    ld g8, 10, sp  ; lcd
    ld g9, 12, sp  ; x
    ld g10, 14, sp  ; y
    ld g11, 16, sp  ; num

    push g3
    push g4
    push g5

    ; lcd_fill_rect(lcd, x, y, 1, 5, 0)
    ld g0, #0
    push g0
    ld g0, #5
    push g0
    ld g0, #1
    push g0
    push g10
    push g9
    push g8
    call _lcd_fill_rect

    ; lcd_fill_rect(lcd, x + 4, y, 1, 5, 0)
    ld g0, g9  ; x
    add g0, #4  ; x + 4
    st g0, 2, sp
    call _lcd_fill_rect
    add sp, #12

    ld g3, g11  ; num
    shll g3, #2  ; num * 4
    add g3, #.icons  ; ptr

    sub sp, #6
    push g8
    ld g5, #0  ; i = 0

.ms_draw_icon_l0
    ld g4, #0  ; j = 0

.ms_draw_icon_l1
    ; shift = 8 - ((i & 1) * 4 + j)
    ; value = (*ptr >> shift) & 1
    ; lcd_draw(lcd, x + j, y + i, value)
    ld g0, @g3  ; *ptr
    ld g1, g5  ; i
    and g1, #1  ; bits: 000i
    shll g1, #2  ; bits: 0i00
    or g1, g4  ; bits: 0ijj
    xor g1, #7
    shrl g0, g1
    and g0, #1
    st g0, 6, sp
    ld g0, g10
    add g0, g5
    st g0, 4, sp
    ld g0, g9
    add g0, g4
    st g0, 2, sp
    call _lcd_draw

    add g4, #1  ; j++
    cmp g4, #4  ; j == 4
    jnz .ms_draw_icon_l1

    add g5, #1  ; i++
    ld g0, g5  ; i
    and g0, #1  ; i & 1
    jnz .ms_draw_icon_l2
    add g3, #1  ; ptr++

.ms_draw_icon_l2
    cmp g5, #5  ; i == 5
    jnz .ms_draw_icon_l0

    add sp, #8

    cmp g11, #10  ; num == MS_MINE_MASK
    jnz .ms_draw_icon_l3

    ; lcd_draw(lcd, x + 4, y + 2, 1)
    ld g0, #1
    push g0
    ld g0, g10
    add g0, #2
    push g0
    ld g0, g9
    add g0, #4
    push g0
    push g8
    call _lcd_draw
    add sp, #8

.ms_draw_icon_l3
    pop g5
    pop g4
    pop g3
    pop g11
    pop g10
    pop g9
    pop g8
    ret
