; int call(unsigned int func, unsigned int argc, ...)
_call
    push fp
    ld fp, sp

    ld g1, 4, fp  ; func
    ld g2, 6, fp  ; argc
    add fp, #8

.call_l0
    sub g2, #1
    jc .call_l1

    ld g0, 0, fp
    st g0, -4, fp
    add fp, #2
    jmp .call_l0

.call_l1
    pop fp
    jmp 0, g1

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

; unsigned int mulul(unsigned int l, unsigned int r)
_mulul
    push fp
    ld fp, sp

    add fp, #4  ; &l1
    ld g0, @fp  ; l1
    add fp, #3  ; &r0
    mul g0, @fp  ; l1 * r0
    push g0  ; c = l1 * r0
    and g0, #255  ; c & 255

    ld g1, @fp  ; r0
    sub fp, #2  ; &l0
    mul g1, @fp  ; a = l0 * r0
    shrl g1, #8  ; a >> 8
    add g0, g1  ; (a >> 8) + (c & 255)

    ld g1, @fp  ; l0
    add fp, #1  ; &r1
    mul g1, @fp  ; b = l0 * r1
    add g0, g1  ; y = (a >> 8) + b + (c & 255)
    shrl g0, #8  ; y >> 8

    ld g1, @fp  ; r1
    sub fp, #2  ; &l1
    mul g1, @fp  ; d = l1 * r1
    add g0, g1  ; (y >> 8) + d

    sub fp, #6  ; &c1
    add g0, @fp  ; y = (y >> 8) + (c >> 8) + d

    add sp, #2
    pop fp
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
