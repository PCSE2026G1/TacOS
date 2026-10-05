#include "home.h"
#include "cmmdef.h"
#include "cmm.h"
#include "key.h"

extern FPTR(void, PTR(home_functions_ptr));
extern const char PTR(PTR(home_titles_ptr));

extern void home_show(VOID)
{
    spiResetLcd();
    int size = subpp2(home_titles_ptr, home_functions_ptr);
    while (COND(1))
    {
        int i = select("HOME", home_titles_ptr, size);
        if (i >= 0 && i < size)
        {
            while (COND(key_read()))
                sleep(10);
            key_step();
            key_step();
            CALL0(home_functions_ptr[i]);
        }
    }
}
