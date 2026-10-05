.title_files string "Files"
.title_playlists string "Playlists"
.title_history string "History"
.title_clock string "Clock"
.title_now_playing string "Now Playing"

.functions dw _mp3_show_files
    dw _mp3_show_playlists
    dw _mp3_show_history
    dw _clock_show
    dw _mp3_show_now_playing
.titles dw .title_files
    dw .title_playlists
    dw .title_history
    dw .title_clock
    dw .title_now_playing

_home_functions_ptr dw .functions
_home_titles_ptr dw .titles
