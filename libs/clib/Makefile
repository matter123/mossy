#custom make file for integrating Mossy2 and CLIB
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o.$(OBJ_SUFFIX), $(CSOURCES))
CXXSOURCES:=$(shell find . -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o.$(OBJ_SUFFIX), $(CXXSOURCES))

build: $(COBJECTS) $(CXXOBJECTS) mklib

%.o.$(OBJ_SUFFIX): %.c
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) \
	-pedantic -o libs/clib/$@ -c libs/clib/$<

%.o.$(OBJ_SUFFIX): %.cpp
	@echo Compiling $<
	@cd ../.. && $(CXX) $(CXXFLAGS) \
	-pedantic -o libs/clib/$@ -c libs/clib/$<

mklib:
	@rm -f ../libc$(OBJ_SUFFIX).a
	@$(AR) rcs ../libc$(OBJ_SUFFIX).a $(COBJECTS) $(CXXOBJECTS)

clean:
	@echo cleaning CLIB
	@rm -f ../libc$(OBJ_SUFFIX).a
	find . -name '*.o.*' -delete
