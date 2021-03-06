ARCH=x86_64
LIBC_BIN_PATH=../../bin/$(ARCH)/libc
LIBK_BIN_PATH=../../bin/$(ARCH)/libk
SYSROOT_PATH=../../sysroot
LIBC=../../sysroot/usr/lib/libc.a
LIBK=../../sysroot/usr/lib/libk.a


CSOURCES:=$(shell find . -name '*.c')
COBJECTS:=$(patsubst %.c, %.c.o, $(CSOURCES))
LC_COBJECTS:=$(patsubst ./%, $(LIBC_BIN_PATH)/%, $(COBJECTS))
LK_COBJECTS:=$(patsubst ./%, $(LIBK_BIN_PATH)/%, $(COBJECTS))

CXXSOURCES:=$(shell find . -name '*.cpp')
CXXOBJECTS:=$(patsubst %.cpp, %.cpp.o, $(CXXSOURCES))
LC_CXXOBJECTS:=$(patsubst ./%, $(LIBC_BIN_PATH)/%, $(CXXOBJECTS))
LK_CXXOBJECTS:=$(patsubst ./%, $(LIBK_BIN_PATH)/%, $(CXXOBJECTS))

SSOURCES:=$(shell find . -name '*.asm')
SOBJECTS:=$(patsubst %.asm, %.asm.o, $(SSOURCES))
LC_SOBJECTS:=$(patsubst ./%, $(LIBC_BIN_PATH)/%, $(SOBJECTS))
LK_SOBJECTS:=$(patsubst ./%, $(LIBK_BIN_PATH)/%, $(SOBJECTS))

HEADSRC:=$(shell find . -name '*.h'|grep include)
HEADSYSROOT:=$(patsubst ./%, $(SYSROOT_PATH)/usr/include/libc/%, $(HEADSRC))


WFLAGSON=-Wall -Wextra -Wreturn-type -Werror=return-type -Wshadow -Wundef -Wdeprecated -Wredundant-decls -Werror=parentheses
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-implicit-exception-spec-mismatch
FFLAGS=-ffreestanding -fstack-protector -fno-common
DFLAGS:=-DMOSSY $(BUILD_INFO)
MFLAGS=-mcmodel=kernel -mno-red-zone
INCLUDEPATHS=$(wildcard $(SYSROOT_PATH)/usr/include/*/include)
IFLAGS=--sysroot=$(SYSROOT_PATH) $(addprefix -I, $(INCLUDEPATHS)) -include $(SYSROOT_PATH)/usr/include/kernel/stdlib/include/global.h
CFLAGS=-O3 -std=c11 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS)
CXXFLAGS=-O3 -std=c++14 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS) -fno-rtti -fno-exceptions -fdiagnostics-color=auto
ASFLAGS=-felf64

all: $(LIBC) $(LIBK)
.PHONY: all headers clean
headers: $(HEADSYSROOT)
clean:
	@-rm $(LIBC) $(LIBK) 2> /dev/null
	@-rm $(LK_CXXOBJECTS) $(LK_COBJECTS) $(LK_SOBJECTS) $(LC_CXXOBJECTS) $(LC_COBJECTS) $(LC_SOBJECTS) 2> /dev/null

$(LIBC): $(LC_CXXOBJECTS) $(LC_COBJECTS) $(LC_SOBJECTS)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AR) rcs $@ $^

$(LIBK): $(LK_CXXOBJECTS) $(LK_COBJECTS) $(LK_SOBJECTS)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AR) rcs $@ $^

$(LIBC_BIN_PATH)/%.cpp.o: ./%.cpp $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CPP) -DLIBC $(CXXFLAGS) -o $@ -c $<
$(LIBK_BIN_PATH)/%.cpp.o: ./%.cpp $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CPP) -DLIBK $(CXXFLAGS) -o $@ -c $<

$(LIBC_BIN_PATH)/%.c.o: ./%.c $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CC) -DLIBC $(CFLAGS) -o $@ -c $<
$(LIBK_BIN_PATH)/%.c.o: ./%.c $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CC) -DLIBK $(CFLAGS) -o $@ -c $<

$(LIBC_BIN_PATH)/%.asm.o: ./%.asm
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AS) -dLIBC $(ASFLAGS) $< -o $@
$(LIBK_BIN_PATH)/%.asm.o: ./%.asm
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AS) -dLIBK $(ASFLAGS) $< -o $@

.PRECIOUS: $(HEADSYSROOT)
$(SYSROOT_PATH)/usr/include/libc/%.h: ./%.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@echo $@ $<
