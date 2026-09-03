; void* addp(const void* l, int r)
_addp
    ld g0, 2, sp
    add g0, 4, sp
    ret

; void* subp(const void* l, int r)
; int subpp(const void* l, const void* r)
_subp
_subpp
    ld g0, 2, sp
    sub g0, 4, sp
    ret

; unsigned int getp(const void* p)
_getp
    ld g0, 2, sp
    ld g0, @g0
    ret

; void setp(void* p, int value)
_setp
    ld g0, 4, sp
    ld g1, 2, sp
    st g0, @g1
    ret

; void* addp2(const void* l, int r)
_addp2
    ld g0, 4, sp
    shll g0, #1
    add g0, 2, sp
    ret

; void* subp2(const void* l, int r)
_subp2
    ld g0, 2, sp
    sub g0, 4, sp
    sub g0, 4, sp
    ret

; int subpp2(const void* l, const void* r)
_subpp2
    ld g0, 2, sp
    sub g0, 4, sp
    shra g0, #1
    ret

; unsigned int getp2(const void* p)
_getp2
    ld g0, 2, sp
    ld g0, 0, g0
    ret

; void setp2(void* p, unsigned int value)
_setp2
    ld g0, 4, sp
    ld g1, 2, sp
    st g0, 0, g1
    ret

; int cmpu(unsigned int l, unsigned int r)
; int cmpp(const void* l, const void* r)
_cmpu
_cmpp
    ld g0, 2, sp
    sub g0, 4, sp
    jc .cmpu_c
    jz .cmpu_ret
    ld g0, #1
    jmp .cmpu_ret

.cmpu_c
    ld g0, #-1

.cmpu_ret
    ret

; unsigned int cond3(int cond, unsigned int l, unsigned int r)
_cond3
    ld g0, 2, sp
    cmp g0, #0
    jz .cond3_l0
    ld g0, 4, sp
    jmp .cond3_l1

.cond3_l0
    ld g0, 6, sp

.cond3_l1
    ret
