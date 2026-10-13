#ifndef CMMDEF_H
#define CMMDEF_H

/*
 * @startuml(id=cmmdef)
 * class cmmdef {
 *     +void* addp(const void* l, int r)
 *     +void* subp(const void* l, int r)
 *     +int subpp(const void* l, const void* r)
 *     +unsigned int getp(const void* p)
 *     +void setp(void* p, int value)
 *     +void* addp2(const void* l, int r)
 *     +void* subp2(const void* l, int r)
 *     +int subpp2(const void* l, const void* r)
 *     +unsigned int getp2(const void* p)
 *     +void setp2(void* p, unsigned int value)
 *     +int cmpu(unsigned int l, unsigned int r)
 *     +int cmpp(const void* l, const void* r)
 *     +unsigned int mulul(unsigned int l, unsigned int r)
 *     +unsigned int cond3(int cond, unsigned int l, unsigned int r)
 * }
 * @enduml
 *
 * @startuml
 * () ABI as i1
 * () ABI as i2
 *
 * [Caller] - i1
 * i2 - [Callee]
 * [call function] ..> i1
 * [call function] ..> i2
 * @enduml
 *
 * @startuml
 * start
 * :func と argc をロードする;
 * :フレームポインタを引数の最初に設定する;
 * while (argc が 0 より大きい)
 *     :argc から 1 を引く;
 *     :フレームポインタが指す値を 4 バイト前にストアする;
 *     :フレームポインタに 2 を足す;
 * endwhile
 * :フレームポインタを復元する;
 * :func が指すアドレスに移動する;
 * stop
 * @enduml
 *
 * @startuml
 * start
 * :l の上位 8 ビットを l1, 下位 8 ビットを l0 とする
 * r の上位 8 ビットを r1, 下位 8 ビットを r0 とする;
 * :l0 * r0 を a とする
 * l0 * r1 を b とする
 * l1 * r0 を c とする
 * l1 * r1 を d とする;
 * :a の上位 8 ビットと c の下位 8 ビットを足す;
 * :演算結果に b を足す;
 * :演算結果を 8 ビット右シフトする;
 * :演算結果に d を足す;
 * :演算結果に c の上位 8 ビットを足す;
 * :演算結果を返す;
 * stop
 * @enduml
 */

#ifdef CMMDEF
#define extern public
#define static
#endif

#ifdef CMMDEF
#define EXTERN extern
#else
#define EXTERN
#endif

#ifdef CMMDEF
#define signed
#define unsigned
#define const
#define restrict
#endif

#ifdef CMMDEF
#define VOID
#define PTR(x) []x
#define FPTR(t, x, ...) int x
#define STRUCT(t) void
#else
#define VOID void
#define PTR(x) (*x)
#define FPTR(t, x, ...) t (*x)(__VA_ARGS__)
#define STRUCT(t) t
#endif

#ifdef CMMDEF
#define INT_ADDR(x) addrof(x)
#define F_ADDR(x) addrof(x)
#else
#define INT_ADDR(x) ((unsigned int)&(x))
#define F_ADDR(x) (&(x))
#endif

#ifdef CMMDEF
extern int call(...);
#define CALL(x, argc, ...) call(x, argc, __VA_ARGS__)
#define CALL0(x) call(x, 0)
#else
#define CALL(x, argc, ...) ((x)(__VA_ARGS__))
#define CALL0(x) CALL(x)
#endif

#ifdef CMMDEF
#define COND(x) ((x) != 0)
#else
#define COND(x) (x)
#endif
#define AND(l, r) (COND(l) && COND(r))
#define OR(l, r) (COND(l) || COND(r))

#define SHRU(l, r) ((l) >> (r) & (0x7fff >> (r) << 1 | 1))

#ifdef CMMDEF
#define INC(x) ((x) = (x) + 1)
#define DEC(x) ((x) = (x) - 1)
#define ADDA(l, r) ((l) = (l) + (r))
#define SUBA(l, r) ((l) = (l) - (r))
#define MULA(l, r) ((l) = (l) * (r))
#define DIVA(l, r) ((l) = (l) / (r))
#define ANDA(l, r) ((l) = (l) & (r))
#define ORA(l, r) ((l) = (l) | (r))
#define SHRA(l, r) ((l) = (l) >> (r))
#define SHLA(l, r) ((l) = (l) << (r))
#else
#define INC(x) (++(x))
#define DEC(x) (--(x))
#define ADDA(l, r) ((l) += (r))
#define SUBA(l, r) ((l) -= (r))
#define MULA(l, r) ((l) *= (r))
#define DIVA(l, r) ((l) /= (r))
#define ANDA(l, r) ((l) &= (r))
#define ORA(l, r) ((l) |= (r))
#define SHRA(l, r) ((l) >>= (r))
#define SHLA(l, r) ((l) <<= (r))
#endif

#ifdef CMMDEF
#define NULL null
#else
#define NULL ((void*)0)
#endif

extern void PTR(addp(const void PTR(l), int r));
extern void PTR(subp(const void PTR(l), int r));
extern int subpp(const void PTR(l), const void PTR(r));
extern unsigned int getp(const void PTR(p));
extern void setp(void PTR(p), int value);
extern void PTR(addp2(const void PTR(l), int r));
extern void PTR(subp2(const void PTR(l), int r));
extern int subpp2(const void PTR(l), const void PTR(r));
extern unsigned int getp2(const void PTR(p));
extern void setp2(void PTR(p), unsigned int value);
extern int cmpu(unsigned int l, unsigned int r);
extern int cmpp(const void PTR(l), const void PTR(r));

extern unsigned int mulul(unsigned int l, unsigned int r);

#ifdef CMMDEF
extern unsigned int cond3(...);
#else
extern unsigned int cond3(int cond, unsigned int l, unsigned int r);
#endif

#endif
