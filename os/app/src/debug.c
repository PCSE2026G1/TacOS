#include "debug.h"
#include "cmmdef.h"
#include "cmm.h"
#include "assert.h"
#include "string.h"

extern void puts(const char PTR(str))
{
    dbgPutStr(str);
    dbgPutStr("\n");
}

extern void puti(int num)
{
    char PTR(buf) = malloc(8);
    assert(buf != NULL);
    puts(itoa(buf, 8, num));
    free(buf);
}

extern void putx(unsigned int num)
{
    char PTR(buf) = malloc(5);
    assert(buf != NULL);
    puts(itox(buf, 5, num));
    free(buf);
}
