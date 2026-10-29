; Test-only illegal-instruction probe. Does not return.
_playlistFaultProbe
    ld g3,2,sp
    ld g4,g3
    ld g6,#0
    ld g1,#.prefix
.prefixLoop
    ld g0,@g1
    cmp g0,#0
    jz .hexStart
.prefixWait
    in g2,0x0a
    and g2,#0x80
    jz .prefixWait
    out g0,0x08
    add g1,#1
    jmp .prefixLoop
.hexStart
    ld g5,#4
.hexLoop
    ld g0,g3
    shrl g0,#12
    and g0,#0x0f
    ld g1,#.digits
    add g1,g0
    ld g0,@g1
.hexWait
    in g2,0x0a
    and g2,#0x80
    jz .hexWait
    out g0,0x08
    shll g3,#4
    sub g5,#1
    jnz .hexLoop
    cmp g6,#0
    jz .showPrevious
    cmp g6,#1
    jz .showOpcode
    ld g0,#13
.crWait
    in g2,0x0a
    and g2,#0x80
    jz .crWait
    out g0,0x08
    ld g0,#10
.lfWait
    in g2,0x0a
    and g2,#0x80
    jz .lfWait
    out g0,0x08
.stop
    halt
    jmp .stop
.showPrevious
    ld g6,#1
    ld g0,g4
    sub g0,#2
    ld g3,0,g0
    ld g1,#.previousPrefix
    jmp .prefixLoop
.showOpcode
    ld g6,#2
    ld g3,0,g4
    ld g1,#.opcodePrefix
    jmp .prefixLoop
.prefix string "Playlist fault PC="
.previousPrefix string " PREV="
.opcodePrefix string " OP="
.digits string "0123456789abcdef"
