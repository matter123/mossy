#custom make file for integrating Mossy2 and libencode
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CXXSOURCES:=$(shell find . -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o, $(CXXSOURCES))

build: $(COBJECTS) $(CXXOBJECTS) mklib

.c.o:
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) \
	-o libs/encode/$@.$(OBJ_SUFFIX) -c libs/encode/$<

.cpp.o:
	@echo Compiling $<
	@cd ../.. && $(CXX) $(CXXFLAGS) \
	-o libs/encode/$@.$(OBJ_SUFFIX) -c libs/encode/$<

mklib:
	@rm -f ../libencode$(OBJ_SUFFIX).a
	@$(AR) rcs ../libencode$(OBJ_SUFFIX).a *.o.$(OBJ_SUFFIX)

clean:
	@echo cleaning libencode
	@rm -f ../libencode$(OBJ_SUFFIX).a
	find . -name '*.o.*' -delete