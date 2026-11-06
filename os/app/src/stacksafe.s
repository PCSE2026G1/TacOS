; int stacksafe(void* buf, unsigned int size, unsigned int func, unsigned int argc, ...)
_stacksafe
    push fp
    ld fp, sp

    ld sp, 4, fp
    add sp, 6, fp

    ld g1, 10, fp
    cmp g1, #0
    jz .stacksafe_l1

    add fp, #12
    add fp, g1
    add fp, g1

.stacksafe_l0
    sub fp, #2
    ld g0, 0, fp
    push g0
    sub g1, #1
    jnz .stacksafe_l0

    sub fp, #12

.stacksafe_l1
    ld g1, 8, fp
    call 0, g1

    ld sp, 4, fp
    add sp, 6, fp
    push g0

    ld sp, fp
    pop fp
    ret
