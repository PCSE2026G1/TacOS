#ifndef TAC_ASSERT_H
#define TAC_ASSERT_H

#include "cmmdef.h"
#include "cmm.h"
#ifdef CLIB
#include <assert.h>
#endif

#ifndef CLIB
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

#endif
