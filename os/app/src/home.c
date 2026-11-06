#include "home.h"
#include "cmmdef.h"
#include "cmm.h"
#include "lcd.h"
#include "stacksafe.h"

extern FPTR(void, PTR(home_functions_ptr));
extern const char PTR(PTR(home_titles_ptr));
extern void PTR(sf_buf);
extern void showPlaylistViewer(const char PTR(file));
extern void showHistory(VOID);

extern void home_show(VOID)
{
    spiResetLcd();
    int size0 = subpp2(home_titles_ptr, home_functions_ptr);
    while (COND(1))
    {
        int size = size0;
        if (!COND(isMP3PlayerPlaying()))
            DEC(size);
        int i = select("HOME", home_titles_ptr, size);
        if (i >= 0)
            stacksafe(sf_buf, 512, home_functions_ptr[i], 0);
    }
}

extern void mp3_show_files(VOID)
{
    char PTR(path) = malloc(1024);
    if (path == NULL)
        panic("malloc failed\n");
    chooserMp3(path);
    if (path[0] != '\0')
        showMP3PlayerFile(path);
    free(path);
}

extern void mp3_show_playlists(VOID)
{
    // lcd_draw_string(0, 0, "mp3_show_playlists: stubbed");
    // sleep(1000);
    showPlaylistViewer("/PLPLAY.TXT");
}

extern void mp3_show_history(VOID)
{
    showHistory();
}
