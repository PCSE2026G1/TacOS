#ifndef ASSERT_H
#define ASSERT_H

#include "cmmdef.h"
#include "cmm.h"

#ifdef DEBUG
#ifdef CMMDEF
#define assert(x) ((x) || (dbgPutStr(#x), panic("assert"), (x)), 0)
#else
#define assert(x) (void)((x) ? 0 : (dbgPutStr(#x), panic("assert"), 0))
#endif
#else
#ifdef CMMDEF
#define assert(x) 0
#else
#define assert(x) ((void)0)
#endif
#endif

#endif
