#ifndef CMMDEF_H
#define CMMDEF_H

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
#define FPTR(t, x, params) void[]x
#define STRUCT(t) void
#else
#define VOID void
#define PTR(x) (*x)
#define FPTR(t, x, params) t (*x)params
#define STRUCT(t) t
#endif

#ifdef CMMDEF
#define COND(x) ((x) != 0)
#else
#define COND(x) (x)
#endif
#define AND(l, r) (COND(l) && COND(r))
#define OR(l, r) (COND(l) || COND(r))

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

#ifdef CMMDEF
extern unsigned int cond3(...);
#else
extern unsigned int cond3(int cond, unsigned int l, unsigned int r);
#endif

#endif
