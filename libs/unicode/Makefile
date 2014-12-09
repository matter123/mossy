#custom make file for integrating Mossy2 and libunicode
#licensed under Apache 2.0 Public license

CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o.$(OBJ_SUFFIX), $(CSOURCES))
CXXSOURCES:=$(shell find . -name "*.cpp")
CXXOBJECTS:=$(patsubst %.cpp, %.o.$(OBJ_SUFFIX), $(CXXSOURCES))

build: $(COBJECTS) $(CXXOBJECTS) mklib

%.o.$(OBJ_SUFFIX): %.c
	@echo Compiling $<
	@cd ../.. && $(CC) $(CFLAGS) \
	-o libs/unicode/$@ -c libs/unicode/$<

%.o.$(OBJ_SUFFIX): %.cpp
	@echo Compiling $<
	@cd ../.. && $(CXX) $(CXXFLAGS) \
	-o libs/unicode/$@ -c libs/unicode/$<

mklib:
	@rm -f ../libunicode$(OBJ_SUFFIX).a
	@$(AR) rcs ../libunicode$(OBJ_SUFFIX).a $(COBJECTS) $(CXXOBJECTS)

clean:
	@echo cleaning libunicode
	@rm -f ../libunicode$(OBJ_SUFFIX).a
	find . -name '*.o.*' -delete