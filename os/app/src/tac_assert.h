#ifndef TAC_ASSERT_H
#define TAC_ASSERT_H

/*
 * @startuml(id=tac_assert)
 * class tac_assert
 * @enduml
 *
 * cmmdef ..> tac_assert
 */

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
