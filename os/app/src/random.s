; static unsigned int seed[] = {0, 0, 0, 1}
.seed dw 0
    dw 0
    dw 0
    dw 1

; void llsrand(unsigned int x)
_llsrand
    ld g1, #.seed  ; seed

    ld g0, 2, sp  ; x
    st g0, 6, g1  ; seed[3] = x

    ld g0, #0
    st g0, 4, g1  ; seed[2] = 0
    st g0, 2, g1  ; seed[1] = 0
    st g0, 0, g1  ; seed[0] = 0

    ret

; int llrand(void)
_llrand
    ld g2, #.seed  ; (unsigned char*)seed
    add g2, #7  ; &seed[7]
    ld g1, #31  ; carry = 31

.llrand_l0
    ld g0, @g2  ; x = *g2
    mul g0, #17  ; x * 17
    add g0, g1  ; x * 17 + carry
    st g0, @g2  ; *g2 = x * 17 + carry

    ld g1, g0  ; carry = x * 17 + carry
    shrl g1, #8  ; carry >>= 8

    cmp g2, #.seed  ; g2 == seed
    jz .llrand_l1

    sub g2, #1  ; g2--
    jmp .llrand_l0

.llrand_l1
    ld g0, .seed
    shrl g0, #1
    ret
