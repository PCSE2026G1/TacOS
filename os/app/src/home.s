.title_files string "Files"
.title_playlists string "Playlists"
;.title_history string "History"
.title_clock string "Clock"
.title_minesweeper string "Minesweeper"
.title_mahjong string "Mahjong"
.title_playing string "Now Playing"

.functions dw _mp3_show_files
    dw _mp3_show_playlists
    ;dw _mp3_show_history
    dw _showClockViewer
    dw _ms_play
    dw _mj_play
    dw _showCurrentMP3Player
.titles dw .title_files
    dw .title_playlists
    ;dw .title_history
    dw .title_clock
    dw .title_minesweeper
    dw .title_mahjong
    dw .title_playing

_home_functions_ptr dw .functions
_home_titles_ptr dw .titles
