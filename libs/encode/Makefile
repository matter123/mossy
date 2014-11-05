#custom make file for integrating Mossy2 and libencode
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o.$(OBJ_SUFFIX), $(CSOURCES))
CXXSOURCES:=$(shell find . -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o.$(OBJ_SUFFIX), $(CXXSOURCES))

build: $(COBJECTS) $(CXXOBJECTS) mklib

%.o.$(OBJ_SUFFIX): %.c
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) \
	-o libs/encode/$@ -c libs/encode/$<

%.o.$(OBJ_SUFFIX): %.cpp
	@echo Compiling $<
	@cd ../.. && $(CXX) $(CXXFLAGS) \
	-o libs/encode/$@ -c libs/encode/$<

mklib:
	@rm -f ../libencode$(OBJ_SUFFIX).a
	@$(AR) rcs ../libencode$(OBJ_SUFFIX).a $(COBJECTS) $(CXXOBJECTS)

clean:
	@echo cleaning libencode
	@rm -f ../libencode$(OBJ_SUFFIX).a
	find . -name '*.o.*' -delete