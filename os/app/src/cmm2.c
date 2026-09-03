#ifdef CMMDEF
#error "Not C--"
#endif
#include "cmm.h"
#undef sleep
#include <unistd.h>

int clib_sleep(unsigned int ms)
{
    if (sleep(ms / 1000))
        return -1;
    return 0;
}
