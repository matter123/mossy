ARCH=x86_64
BIN_PATH=../../bin/$(ARCH)/kernel
SYSROOT_PATH=../../sysroot
KERNEL=../../sysroot/boot/kernel

CC=clang --target=x86_64-elf
CPP=clang++ --target=x86_64-elf
AS= nasm

CSOURCES:=$(shell find . -name '*.c')
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
COBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(COBJECTS))

CXXSOURCES:=$(shell find . -name '*.cpp')
CXXOBJECTS:=$(patsubst %.cpp, %.o, $(CXXSOURCES))
CXXOBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(CXXOBJECTS))

SSOURCES:=$(shell find . -name '*.s')
SOBJECTS:=$(patsubst %.s, %.o, $(SSOURCES))
SOBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(SOBJECTS))

HEADSRC:=$(shell find . -name '*.h'|grep include)
HEADSYSROOT:=$(patsubst ./%, $(SYSROOT_PATH)/usr/include/kernel/%, $(HEADSRC))


WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated -Wredundant-decls -Werror=parentheses
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-implicit-exception-spec-mismatch
FFLAGS=-ffreestanding -fstack-protector -fno-common
DFLAGS:=-DMOSSY
MFLAGS=-mcmodel=kernel -mno-red-zone
IFLAGS=--sysroot=$(SYSROOT_PATH) -I$(SYSROOT_PATH)/usr/include/kernel/stdlib/include -I$(SYSROOT_PATH)/usr/include/libc/include -include $(SYSROOT_PATH)/usr/include/kernel/stdlib/include/global.h
CFLAGS=-O3 -std=c11 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS)
CXXFLAGS=-O3 -std=c++1y $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS) -fno-rtti -fno-exceptions -fdiagnostics-color=auto
LDFLAGS=-static -T../link.ld -nostdlib $(FFLAGS) -Wl,-z,max-page-size=0x1000
ASFLAGS ?=-felf64

all: $(KERNEL)
.PHONY: all headers clean
headers: $(HEADSYSROOT)
clean:
	@-rm $(KERNEL) 2> /dev/null
	@-rm $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS) 2> /dev/null

$(KERNEL): $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS)
	@mkdir -p $(dir $@)
	$(CPP) $(LDFLAGS) -o $@ $^ -L $(SYSROOT_PATH)/usr/lib -lk -lgcc

$(BIN_PATH)/%.o: ./%.cpp $(HEADSYSROOT)
	@mkdir -p $(dir $@)
	$(CPP) -DKERNEL $(CXXFLAGS) -o $@ -c $<

$(BIN_PATH)/%.o: ./%.c $(HEADSYSROOT)
	@mkdir -p $(dir $@)
	$(CC) -DKERNEL $(CFLAGS) -o $@ -c $<

$(BIN_PATH)/%.o: ./%.s
	@mkdir -p $(dir $@)
	$(AS) -DKERNEL $(ASFLAGS) -o $@ $<

.PRECIOUS: $(HEADSYSROOT)
$(SYSROOT_PATH)/usr/include/kernel/%.h: ./%.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@echo $@ $<
