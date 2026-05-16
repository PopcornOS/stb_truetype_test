CC=gcc

POPCORNOS=mypopcornos/

ifeq ($(OS),Windows_NT)
EXE=.exe
else
EXE=
endif

ifeq ($(CC),cl)
define pop-c
	@# MSVC path: compile to OBJ, then convert to raw binary
	@# Warning: does not play nice with non-inline functions.
	cl /nologo /GS- /Zi /W3 /Od /D UNICODE /D _UNICODE /c $(1) /Fo$(1).obj
	link.exe /NOLOGO /NODEFAULTLIB /ENTRY:pop_main /SUBSYSTEM:NATIVE /OUT:$(1).exe $(1).obj
	objcopy -O binary $(1).exe $(2)
	rm $(1).obj $(1).exe
endef
else
define pop-c
	@# GCC/Clang path: freestanding compile + LD to raw binary
	$(CC) -ffreestanding -fno-stack-protector -nostdlib \
		   -fno-asynchronous-unwind-tables -fshort-wchar \
		   -mno-red-zone -c $(1) -o $(1).o
	ld -nostdlib -T pop.ld $(1).o -o $(1).tmp$(EXE)
	objcopy -O binary $(1).tmp$(EXE) $(2)
	rm $(1).o $(1).tmp$(EXE)
endef
endif

default: setup
	$(call pop-c,main.c,stb_truetype_test.bin)

install: default
	@if [ ! -f $(POPCORNOS)/binstuff/stb_truetype_test/notosans.ttf ]; then \
		echo "installing Noto Sans Regular to /binstuff/stb_truetype_test/notosans.ttf (Popcorn OS)..."; \
		curl -L https://github.com/notofonts/latin-greek-cyrillic/releases/download/NotoSans-v2.015/NotoSans-v2.015.zip -o NotoSans-v2.015.zip; \
		unzip -o NotoSans-v2.015.zip NotoSans/full/ttf/NotoSans-Regular.ttf; \
		mkdir $(POPCORNOS)/binstuff/stb_truetype_test; \
		cp NotoSans/full/ttf/NotoSans-Regular.ttf $(POPCORNOS)/binstuff/stb_truetype_test/notosans.ttf; \
	fi
	cp stb_truetype_test.bin $(POPCORNOS)/system/stb_truetype_test.bin
	rm -rf NotoSans
	rm -f NotoSans-v2.015.zip

setup:
	curl https://raw.githubusercontent.com/PopcornOS/popcorn-os/refs/heads/master/popcorn.h -o popcorn.h
	curl https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_truetype.h -o stb_truetype.h
	curl https://raw.githubusercontent.com/PopcornOS/popcorn-os/refs/heads/master/pop_trig_h.py -o pop_trig_h.py
	python pop_trig_h.py