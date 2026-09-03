#ifdef CMMDEF
#error "Not C--"
#endif
#include "cmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void dbgPutStr(const char* str)
{
    fputs(str, stderr);
}

void clib_panic(const char* msg, ...)
{
    va_list vlist;
    va_start(vlist, msg);
    vfprintf(stderr, msg, vlist);
    va_end(vlist);
    exit(1);
}

unsigned int in(unsigned int p)
{
    fprintf(stderr, "in(%u)\n> 0x", p);
    unsigned int x;
    scanf("%x", &x);
    return x;
}

void out(unsigned int p, unsigned int v)
{
    fprintf(stderr, "out(%u, 0x%04x)\n", p, v);
}

void locateXY(unsigned int x, unsigned int y)
{
    fprintf(stderr, "locateXY(%u, %u)\n", x, y);
}

void putStr(const char* str)
{
    fprintf(stderr, "putStr: %s\n", str);
}

void spiWriteLcdCom(const char* buf)
{
    fprintf(stderr, "spiWriteLcdCom: [");
    for (size_t i = 0; buf[i] != '\xff'; i++)
    {
        if (i)
            fprintf(stderr, ", ");
        fprintf(stderr, "0x%02x", buf[i] & 0xff);
    }
    fprintf(stderr, "]\n");
}

void spiWriteLcdDat(const char* buf, unsigned int len)
{
    fprintf(stderr, "spiWriteLcdDat: [");
    for (unsigned int i = 0; i < len; i++)
    {
        if (i)
            fprintf(stderr, ", ");
        fprintf(stderr, "0x%02x", buf[i] & 0xff);
    }
    fprintf(stderr, "]\n");
}
