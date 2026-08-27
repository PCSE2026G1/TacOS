; void* memset(void* dest, int ch, unsigned int count)
_memset
    ld g0, 4, sp  ; ch
    ld g1, 2, sp  ; dest
    ld g2, 6, sp  ; count

.memset_l0
    st g0, @g1
    add g1, #1
    sub g2, #1
    jnz .memset_l0

    ld g0, 2, sp
    ret
