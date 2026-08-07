#ifndef CMM_H
#define CMM_H

void locateXY(unsigned int x, unsigned int y);
void putStr(const char* str);

void spiWriteLcdCom(const void* buf);

#endif
