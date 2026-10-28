OS_ROOT := $(abspath ../..)

TEST_CPPFLAGS := -DEMBEDDED -DPLAYLIST_UNIT_TEST -nostdinc
TEST_CPPFLAGS += -I/usr/local/cmmInclude -I/usr/cmmInclude
TEST_CPPFLAGS += -I$(OS_ROOT)/kernel
TEST_CPPFLAGS += -I$(OS_ROOT)/util
TEST_CPPFLAGS += -I$(OS_ROOT)/app/src
TEST_CPPFLAGS += -I$(OS_ROOT)/mm
TEST_CPPFLAGS += -I$(OS_ROOT)/fs
TEST_CPPFLAGS += -I$(OS_ROOT)/mp3Player
TEST_CPPFLAGS += -I$(OS_ROOT)/list
TEST_CPPFLAGS += -I$(OS_ROOT)/file

.PHONY: check-entry

check-entry: build/HomePlaylistTest.o

build/HomePlaylistTest.o: HomePlaylistTest.cmm ../Playlist.hmm
	mkdir -p build
	cc -x c -E -P $(TEST_CPPFLAGS) HomePlaylistTest.cmm -o build/HomePlaylistTest.cmm
	cd build && cm2e -c -K -DEMBEDDED -DPLAYLIST_UNIT_TEST -nostdinc HomePlaylistTest.cmm

.PHONY: check-home

check-home: build/HomeForPlaylistTest.o

build/HomeForPlaylistTest.o: $(OS_ROOT)/app/src/home.c
	mkdir -p build
	cc -x c -E -P $(TEST_CPPFLAGS) -DCMMDEF \
		-Dmp3_show_playlists=home_playlist_stub \
		"$(OS_ROOT)/app/src/home.c" \
		-o build/HomeForPlaylistTest.cmm
	cd build && cm2e -c -K -DEMBEDDED -DPLAYLIST_UNIT_TEST -nostdinc HomeForPlaylistTest.cmm


.PHONY: check-playlist

check-playlist: build/Playlist.o build/PlaylistFault.o build/HomeStackTestAdapter.o

# build/Playlist.o: ../Playlist.cmm ../Playlist.hmm GNUmakefile
# 	mkdir -p build
# 	cc -x c -E -P $(TEST_CPPFLAGS) ../Playlist.cmm -o build/Playlist.cmm
# 	cd build && cm2e -c -K -DEMBEDDED -DPLAYLIST_UNIT_TEST -nostdinc Playlist.cmm
build/Playlist.o: ../Playlist.cmm ../Playlist.hmm GNUmakefile
	mkdir -p build
	cc -x c -E -P $(TEST_CPPFLAGS) -UPLAYLIST_UNIT_TEST ../Playlist.cmm -o build/Playlist.cmm
	cd build && cm2e -c -K -DEMBEDDED -nostdinc Playlist.cmm

build/PlaylistFault.o: ../PlaylistFault.s
	mkdir -p build
	cp ../PlaylistFault.s build/PlaylistFault.s
	cd build && as-- PlaylistFault.s

build/HomeStackTestAdapter.o: ../HomeStackTestAdapter.s
	mkdir -p build
	cp ../HomeStackTestAdapter.s build/HomeStackTestAdapter.s
	cd build && as-- HomeStackTestAdapter.s

# 既存アプリの部品を利用する．
# 元のHomeと，結合済みのmod_0.oだけは重複を避けるため除外する．
APP_TEST_OBJS := $(filter-out \
	$(OS_ROOT)/app/build/home.c.o \
	$(OS_ROOT)/app/build/mod_0.o, \
	$(wildcard $(OS_ROOT)/app/build/*.o))

.PHONY: check-app

check-app: build/AppForPlaylistTest.o

build/AppForPlaylistTest.o: $(APP_TEST_OBJS) build/HomeForPlaylistTest.o
	test -f "$(OS_ROOT)/app/build/home.s.o"
	test -f "$(OS_ROOT)/app/build/mp3Proc.cmm.o"
	ld-- $@ $(APP_TEST_OBJS) build/HomeForPlaylistTest.o > build/AppForPlaylistTest.sym


# utilは起動処理を含むため，先頭に置く．
KERNEL_TEST_OBJS := $(OS_ROOT)/util/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/kernel/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/fs/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/mm/mod_0.o
KERNEL_TEST_OBJS += build/AppForPlaylistTest.o
KERNEL_TEST_OBJS += $(OS_ROOT)/file/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/list/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/audioDecoder/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/clock/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/mp3Player/mod_0.o
KERNEL_TEST_OBJS += $(OS_ROOT)/clockViewer/mod_0.o
KERNEL_TEST_OBJS += build/Playlist.o
# KERNEL_TEST_OBJS += build/HomePlaylistTest.o
KERNEL_TEST_OBJS += build/PlaylistViewer.o
KERNEL_TEST_OBJS += build/HomePlaylistPlaybackTest.o
# KERNEL_TEST_OBJS += build/PlaylistFault.o
KERNEL_TEST_OBJS += build/HomeStackTestAdapter.o

.PHONY: test-kernel

test-kernel: build/kernel0.bin

build/kernel0.o: $(KERNEL_TEST_OBJS) GNUmakefile
	ld-- $@ $(KERNEL_TEST_OBJS) > build/kernel0.sym
	size-- $@

build/kernel0.bin: build/kernel0.o
	objbin-- 0x0000 $@ $< > build/kernel0.map
	test -s $@

.PHONY: check-viewer

check-viewer: build/PlaylistViewer.o

build/PlaylistViewer.o: ../PlaylistViewer.cmm ../PlaylistViewer.hmm ../Playlist.hmm $(OS_ROOT)/mp3Player/MP3Player.hmm GNUmakefile
	mkdir -p build
	cc -x c -E -P $(TEST_CPPFLAGS) ../PlaylistViewer.cmm -o build/PlaylistViewer.cmm
	cd build && cm2e -c -K -DEMBEDDED -nostdinc PlaylistViewer.cmm

build/HomePlaylistPlaybackTest.o: HomePlaylistPlaybackTest.cmm ../PlaylistViewer.hmm GNUmakefile
	mkdir -p build
	cc -x c -E -P $(TEST_CPPFLAGS) HomePlaylistPlaybackTest.cmm -o build/HomePlaylistPlaybackTest.cmm
	cd build && cm2e -c -K -DEMBEDDED -nostdinc HomePlaylistPlaybackTest.cmm