#ifndef ASSERT_H
#define ASSERT_H

#include "cmmdef.h"
#include "cmm.h"

#ifdef DEBUG
#ifdef CMMDEF
#define assert(x) ((x) || (dbgPutStr("assert("), dbgPutStr(#x), dbgPutStr(")\n"), panic("assert"), (x)), 0)
#else
#define assert(x) (void)((x) ? 0 : (dbgPutStr("assert("), dbgPutStr(#x), dbgPutStr(")\n"), panic("assert"), 0))
#endif
#else
#ifdef CMMDEF
#define assert(x) 0
#else
#define assert(x) ((void)0)
#endif
#endif

#endif
