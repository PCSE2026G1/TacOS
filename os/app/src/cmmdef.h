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

#endif
