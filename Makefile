CSOURCES=$(shell find -name *.c)
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
CPPSOURCES=$(shell find -name *.cpp)
CPPOBJECTS=$(patsubst %.cpp, %.o, $(CPPSOURCES))
SSOURCES=$(shell find -name *.s)
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

CC=$$HOME/opt/cross/bin/i586-elf-gcc
CPP=$$HOME/opt/cross/bin/i586-elf-gcc
LD=$$HOME/opt/cross/bin/i586-elf-gcc
AS=$$HOME/opt/cross/bin/i586-elf-as

CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

all: build test

build: clean check globalc $(COBJECTS) $(CPPOBJECTS) $(SOBJECTS) link

test: mkiso runem


WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wframe-larger-than=16384 -Wdeprecated -Wredundant-decls -pedantic
WFLAGSOFF=-Wno-sequence-point -Wno-unused-parameter
DFLAGS=-DCPU=586
CFLAGS=-Isrc/stdlib/include -ffreestanding -O2 -std=c99 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF)
CPPFLAGS=-Isrc/stdlib/include -ffreestanding -O2 -std=c++11 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF)
LDFLAGS=-Tlink.ld -ffreestanding -O2 -nostdlib
ASFLAGS=-felf

clean:
	@echo Cleaning workspace
	find . -name '*.o' -delete
	@rm -f kernel

check:
	tools/doccheck.py kernel src
link:
	@echo Linking
	$(LD) $(LDFLAGS) -o kernel ./gc/crti.o $(CRTBEGIN_OBJ) $(SOBJECTS) $(COBJECTS) $(CPPOBJECTS) $(CRTEND_OBJ) ./gc/crtn.o

.s.o:
	@echo Assembling $<
	@nasm $(ASFLAGS) $< -o $@

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<

.cpp.o:
	@echo Compiling $<
	@$(CPP) $(CPPFLAGS) -o $@ -c $<

mkiso:
	#@tools/packrd.py
	@cp -f kernel iso/kernel
	#@cp -f initrd.rd iso/initrd.rd
	@grub2-mkrescue -o bootable.iso iso -- -zisofs level=6

runem:
	@cd .&& bash scripts/run_emulator `pwd` &

globalc:
	@echo assembling help files
	$(AS) ./gc/crti.asm -o ./gc/crti.o
	$(AS) ./gc/crtn.asm -o ./gc/crtn.o
