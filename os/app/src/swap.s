; void swap(void *l, void *r, unsigned int size)
_swap
    push g3
    push g4
    push g5

    ld g3, 8, sp  ; l
    ld g4, 10, sp  ; r
    ld g5, 12, sp  ; size

.swap_l0
    ld g0, @g3
    ld g1, @g4
    st g1, @g3
    st g0, @g4

    add g3, #1
    add g4, #1
    sub g5, #1
    jnz .swap_l0

    pop g5
    pop g4
    pop g3
    ret
