ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
	SYSTEM_DIR := $(SystemRoot)\System32
else ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
	SYSTEM_DIR := $(SystemRoot)\Sysnative
else
	SYSTEM_DIR := $(SystemRoot)\System32
endif

.PHONY: all clean install uninstall

all: ls.exe lsd.exe

clean:
	del obj

ls.exe: obj/ls.o obj/lsd.o obj/FileInfo.o
	gcc obj/ls.o obj/FileInfo.o -municode -lshlwapi -o ls.exe

lsd.exe: obj/ls.o obj/lsd.o obj/FileInfo.o
	gcc obj/lsd.o obj/FileInfo.o -municode -lshlwapi -o lsd.exe

obj/FileInfo.o: include/FileInfo.c
	gcc -c include/FileInfo.c -o obj/FileInfo.o

obj/ls.o: src/ls.c
	gcc -c src/ls.c -o obj/ls.o

obj/lsd.o: src/lsd.c
	gcc -c src/lsd.c -o obj/lsd.o

install: ls.exe lsd.exe
	@echo Checking admin privileges...
	@net session >nul 2>&1 || (echo Error: Run as administrator to install. && exit 1)
	@echo Installing to $(SYSTEM_DIR)
	@copy ls.exe "$(SYSTEM_DIR)" 
	@copy lsd.exe "$(SYSTEM_DIR)"
	@echo Installation complete. Added to $(SYSTEM_DIR)

uninstall:
	@del "$(SYSTEM_DIR)\ls.exe" >nul 2>&1 || echo ls.exe not found
	@del "$(SYSTEM_DIR)\lsd.exe" >nul 2>&1 || echo lsd.exe not found
	@echo Uninstalled from $(SYSTEM_DIR)
