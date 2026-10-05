#ifndef CMM_H
#define CMM_H

#include "cmmdef.h"

extern void dbgPutStr(const char PTR(str));
extern void panic(const char PTR(msg), ...);

extern unsigned int in(unsigned int p);
extern void out(unsigned int p, unsigned int v);

extern void PTR(malloc(unsigned int size));
extern void free(void PTR(ptr));
extern int sleep(unsigned int ms);

extern unsigned int strLen(const char PTR(str));
extern int open(const char PTR(path), unsigned int mode);
extern int close(int fd);
extern int read(int fd, void PTR(buf), unsigned int len);
extern int write(int fd, const void PTR(buf), unsigned int len);
extern int creat(const char PTR(path));
extern int remove(const char PTR(path));

extern void locateXY(unsigned int x, unsigned int y);
extern void putStr(const char PTR(str));
extern void lcd_draw_string(unsigned int x, unsigned int y, const char PTR(str));

extern void spiResetLcd(VOID);
extern void spiWriteLcdCom(const char PTR(buf));
extern void spiWriteLcdDat(const char PTR(buf), unsigned int len);

extern int select(const char PTR(title), const char PTR(const PTR(fnames)), int size);
extern void chooser(char PTR(filename));
extern void chooserMp3(char PTR(filename));
extern void mp3_play_path(char PTR(path));
extern void mp3_play_playlist(char PTR(PTR(paths)), unsigned int count, unsigned int selected);
extern void mp3PlayerTick(VOID);
extern void frontendAlarmTick(VOID);

#endif
