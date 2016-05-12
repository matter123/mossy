ARCH=x86_64
BIN_PATH=../../bin/$(ARCH)/kernel
SYSROOT_PATH=../../sysroot
KERNEL=../../sysroot/boot/kernel

LIBS:=$(wildcard ../lib*)
LIBS:=$(subst ../lib,-l,$(LIBS))
LIBS:=$(subst -lc,-lk,$(LIBS))

CSOURCES:=$(shell find . -name '*.c')
COBJECTS:=$(patsubst %.c, %.c.o, $(CSOURCES))
COBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(COBJECTS))

CXXSOURCES:=$(shell find . -name '*.cpp')
CXXOBJECTS:=$(patsubst %.cpp, %.cpp.o, $(CXXSOURCES))
CXXOBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(CXXOBJECTS))

SSOURCES:=$(shell find . -name '*.asm')
SOBJECTS:=$(patsubst %.asm, %.asm.o, $(SSOURCES))
SOBJECTS:=$(patsubst ./%, $(BIN_PATH)/%, $(SOBJECTS))

HEADSRC:=$(shell find . -name '*.h'|grep include)
HEADSYSROOT:=$(patsubst ./%, $(SYSROOT_PATH)/usr/include/kernel/%, $(HEADSRC))

HPPHEADSRC:=$(shell find . -name '*.hpp'|grep include)
HEADSYSROOT += $(patsubst ./%.hpp, $(SYSROOT_PATH)/usr/include/kernel/%, $(HPPHEADSRC))

WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated -Wredundant-decls -Werror=parentheses -pedantic
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-implicit-exception-spec-mismatch
FFLAGS=-ffreestanding -fstack-protector -fno-common
DFLAGS:=-DKERNEL -DMOSSY $(BUILD_INFO)
MFLAGS=-mcmodel=kernel -mno-red-zone
INCLUDEPATHS:=$(wildcard $(SYSROOT_PATH)/usr/include/kernel/*/include)
INCLUDEPATHS:=$(INCLUDEPATHS) $(wildcard $(SYSROOT_PATH)/usr/include/*/include)
INCLUDEPATHS:=$(addprefix -I, $(INCLUDEPATHS))
IFLAGS=--sysroot=$(SYSROOT_PATH) $(INCLUDEPATHS) -include $(SYSROOT_PATH)/usr/include/kernel/stdlib/include/global.h
CFLAGS=-O3 -std=c11 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS)
CXXFLAGS=-O3 -std=c++14 $(DFLAGS) $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS) $(MFLAGS) $(IFLAGS) -fno-rtti -fno-exceptions -fdiagnostics-color=auto
LDFLAGS=-static -T../link.ld -nostdlib $(FFLAGS) -Wl,-z,max-page-size=0x1000 -mcmodel=kernel
ASFLAGS ?=-felf64

all: ../.clang_complete $(KERNEL)
.PHONY: all headers clean
headers: $(HEADSYSROOT)
clean:
	@-rm $(KERNEL) 2> /dev/null
	@-rm $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS) 2> /dev/null

$(KERNEL): $(CXXOBJECTS) $(COBJECTS) $(SOBJECTS)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CPP) $(LDFLAGS) -o $@ $^ -L $(SYSROOT_PATH)/usr/lib $(LIBS) $(LIBS) -lgcc

$(BIN_PATH)/%.cpp.o: ./%.cpp $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CPP) $(CXXFLAGS) -o $@ -c $<

$(BIN_PATH)/%.c.o: ./%.c $(HEADSYSROOT)
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(CC) $(CFLAGS) -o $@ -c $<

$(BIN_PATH)/%.asm.o: ./%.asm
	@echo building $@
	@mkdir -p $(dir $@)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

.PRECIOUS: $(HEADSYSROOT)
$(SYSROOT_PATH)/usr/include/kernel/%.h: ./%.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@echo $@ $<

$(SYSROOT_PATH)/usr/include/kernel/%: ./%.hpp
	@mkdir -p $(dir $@)
	@cp $< $@
	@echo $@ $<

../.clang_complete: $(HEADSYSROOT)
	@echo --target=x86_64-elf> ../.clang_complete
	@echo -std=c++14 >> ../.clang_complete
	@echo $(DFLAGS) | tr " " "\n" >> ../.clang_complete
	@echo $(subst ../../,../,$(IFLAGS)) | tr " " "\n" >> ../.clang_complete
	@echo -ffreestanding $(WFLAGSON) $(WFLAGSOFF) | tr " " "\n" >> ../.clang_complete
