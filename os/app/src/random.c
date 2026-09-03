#ifdef CMMDEF
#error "Not C--"
#endif
#include "random.h"

static unsigned long long seed = 1;

void llsrand(unsigned int x)
{
    seed = x;
}

int llrand(void)
{
    seed = seed * 17 + 31;
    return seed >> 49;
}
