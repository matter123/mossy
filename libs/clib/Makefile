#custom make file for integrating Mossy2 and CLIB
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CPPSOURCES:=$(shell find . -name "*.cpp")
CPPOBJECTS:=$(patsubst %.cpp, %.o, $(CPPSOURCES))

build: $(COBJECTS) $(CPPOBJECTS) mklib

.c.o:
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) -fno-tree-loop-distribute-patterns \
	-pedantic -o libs/clib/$@.$(ARCH) -c libs/clib/$<

.cpp.o:
	@echo Compiling $<
	@cd ../.. && $(CPP) $(CPPFLAGS) -fno-tree-loop-distribute-patterns \
	-pedantic -o libs/clib/$@.$(ARCH) -c libs/clib/$<

mklib:
	@rm -f ../libc$(ARCH).a
	@$(GAR) rcs ../libc$(ARCH).a *.o.$(ARCH)
