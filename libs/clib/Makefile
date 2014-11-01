#custom make file for integrating Mossy2 and CLIB
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CXXSOURCES:=$(shell find . -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o, $(CXXSOURCES))

build: $(COBJECTS) $(CXXOBJECTS) mklib

.c.o:
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) \
	-pedantic -o libs/clib/$@.$(OBJ_SUFFIX) -c libs/clib/$<

.cpp.o:
	@echo Compiling $<
	@cd ../.. && $(CXX) $(CXXFLAGS) \
	-pedantic -o libs/clib/$@.$(OBJ_SUFFIX) -c libs/clib/$<

mklib:
	@rm -f ../libc$(OBJ_SUFFIX).a
	@$(AR) rcs ../libc$(OBJ_SUFFIX).a *.o.$(OBJ_SUFFIX)

clean:
	@echo cleaning CLIB
	@rm -f ../libc$(OBJ_SUFFIX).a
	find . -name '*.o.*' -delete
