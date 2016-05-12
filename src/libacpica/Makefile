ARCH=x86_64
LIB_NAME:=$(notdir $(shell pwd))
LIB_DEFINE:=$(shell echo $(LIB_NAME)|tr [:lower:] [:upper:])
SYSROOT_PATH=../../sysroot

LIB=$(SYSROOT_PATH)/usr/lib/$(LIB_NAME).a
LIB_BIN_PATH=../../bin/$(ARCH)/$(LIB_NAME)


CSOURCES:=$(shell find . -name '*.c')
COBJECTS:=$(patsubst %.c, %.c.o, $(CSOURCES))
COBJECTS:=$(patsubst ./%, $(LIB_BIN_PATH)/%, $(COBJECTS))

CXXSOURCES:=$(shell find . -name '*.cpp')
CXXOBJECTS:=$(patsubst %.cpp, %.cpp.o, $(CXXSOURCES))
CXXOBJECTS:=$(patsubst ./%, $(LIB_BIN_PATH)/%, $(CXXOBJECTS))

SSOURCES:=$(shell find . -name '*.asm')
SOBJECTS:=$(patsubst %.asm, %.asm.o, $(SSOURCES))
SOBJECTS:=$(patsubst ./%, $(LIB_BIN_PATH)/%, $(SOBJECTS))

HEADSRC:=$(shell find . -name '*.h'|grep include)
HEADSYSROOT:=$(patsubst ./%, $(SYSROOT_PATH)/usr/include/$(LIB_NAME)/%, $(HEADSRC))

WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated -Wredundant-decls -Werror=parentheses
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-implicit-exception-spec-mismatch
FFLAGS=-ffreestanding -fstack-protector -fno-common
DFLAGS:=-DMOSSY $(BUILD_INFO)
MFLAGS=-mcmodel=kernel -mno-red-zone
INCLUDEPATHS=$(wildcard $(SYSROOT_PATH)/usr/include/*/include)
IFLAGS=--sysroot=$(SYSROOT_PATH) $(addprefix -I, $(INCLUDEPATHS)) -include $(SYSROOT_PATH)/usr/include/kernel/stdlib/include/global.h
CFLAGS=-O3 -std=c11 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS)
CXXFLAGS=-O3 -std=c++14 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS) -fno-rtti -fno-exceptions -fdiagnostics-color=auto
ASFLAGS=-felf64

all: $(LIB)
.PHONY: all headers clean
headers: $(HEADSYSROOT)
clean:
	@-rm $(LIB) 2> /dev/null
	@-rm $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS) 2> /dev/null

$(LIB): $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AR) rcs $@ $^

$(LIB_BIN_PATH)/%.cpp.o: ./%.cpp $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CPP) -D$(LIB_DEFINE) $(CXXFLAGS) -o $@ -c $<

$(LIB_BIN_PATH)/%.c.o: ./%.c $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CC) -D$(LIB_DEFINE) $(CFLAGS) -o $@ -c $<

$(LIB_BIN_PATH)/%.asm.o: ./%.asm
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AS) -d$(LIB_DEFINE) $(ASFLAGS) $< -o $@

.PRECIOUS: $(HEADSYSROOT)
$(SYSROOT_PATH)/usr/include/$(LIB_NAME)/%.h: ./%.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@echo $@ $<
