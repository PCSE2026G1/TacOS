OS_ROOT ?= $(abspath ../..)
REV := $(shell git -C "$(OS_ROOT)/.." rev-parse --short HEAD)
SHARED := /private/tmp/TacOS-history-shared-$(REV)
SHARED_OS := $(SHARED)/os
STAMP := $(SHARED)/.ready

CPPFLAGS_TEST := -DEMBEDDED -DHISTORY_UNIT_TEST -nostdinc
CPPFLAGS_TEST += -I/usr/local/cmmInclude -I/usr/cmmInclude
CPPFLAGS_TEST += -I$(SHARED_OS)/kernel -I$(SHARED_OS)/util
CPPFLAGS_TEST += -I$(SHARED_OS)/fs -I$(SHARED_OS)/mm
CPPFLAGS_TEST += -I$(SHARED_OS)/app -I$(SHARED_OS)/app/src
CPPFLAGS_TEST += -I$(OS_ROOT)/history
CPPFLAGS_TEST += -I$(SHARED_OS)/app/build
CPPFLAGS_TEST += -I$(OS_ROOT)/audioDecoder

APP_NAMES := spi.cmm.o font.cmm.o lcd.cmm.o appUtil.cmm.o mp3Proc.cmm.o stacksafe.s.o key.c.o cmmdef.s.o

TEST_OBJS := $(SHARED_OS)/util/mod_0.o
TEST_OBJS += $(SHARED_OS)/kernel/mod_0.o
TEST_OBJS += $(SHARED_OS)/fs/mod_0.o
TEST_OBJS += $(SHARED_OS)/mm/mod_0.o
TEST_OBJS += $(addprefix $(SHARED_OS)/app/build/,$(APP_NAMES))
TEST_OBJS += build/AudioDecoder.o
TEST_OBJS += $(SHARED_OS)/clock/mod_0.o
TEST_OBJS += build/History.o build/HistoryTestMain.o

.PHONY: test-kernel
test-kernel: build/kernel0.bin

# 共有部品はcommit済みソースのコピー内でビルドする．
$(STAMP): GNUmakefile
	mkdir -p "$(SHARED)"
	git -C "$(OS_ROOT)/.." archive HEAD | tar -x -C "$(SHARED)"
	for dir in util kernel fs mm app audioDecoder clock; do \
	  $(MAKE) -C "$(SHARED_OS)/$$dir" EMBEDDED=1 DEBUG= CLOCKVIEWER_UNIT_TEST=1 || exit 1; \
	done
	touch "$@"

build/History.o: $(OS_ROOT)/history/History.cmm $(OS_ROOT)/history/History.hmm GNUmakefile $(STAMP)
	mkdir -p build
	cc -x c -E -P $(CPPFLAGS_TEST) "$<" -o build/History.cmm
	cd build && cm2e -c -K -DEMBEDDED -DHISTORY_UNIT_TEST -nostdinc History.cmm

build/HistoryTestMain.o: HistoryTestMain.cmm $(OS_ROOT)/history/History.hmm GNUmakefile $(STAMP)
	mkdir -p build
	cc -x c -E -P $(CPPFLAGS_TEST) "$<" -o build/HistoryTestMain.cmm
	cd build && cm2e -c -K -DEMBEDDED -DHISTORY_UNIT_TEST -nostdinc HistoryTestMain.cmm

build/AudioDecoder.o: $(OS_ROOT)/audioDecoder/AudioDecoder.cmm $(OS_ROOT)/audioDecoder/AudioDecoder.hmm $(OS_ROOT)/history/History.hmm GNUmakefile $(STAMP)
	mkdir -p build
	cc -x c -E -P $(CPPFLAGS_TEST) -I$(OS_ROOT)/audioDecoder "$<" -o build/AudioDecoder.cmm
	cd build && cm2e -c -K -DEMBEDDED -nostdinc AudioDecoder.cmm

build/kernel0.o: build/History.o build/HistoryTestMain.o build/AudioDecoder.o GNUmakefile $(STAMP)
	ld-- $@ $(TEST_OBJS) > build/kernel0.sym
	size-- $@

build/kernel0.bin: build/kernel0.o
	objbin-- 0x0000 $@ $< > build/kernel0.map
	test -s $@