CSOURCES:=$(shell find . -name *.c)
COBJECTS:=$(patsubst %.c, %.o.$(OBJ_SUFFIX), $(CSOURCES))
CXXSOURCES:=$(shell find . -name *.cpp)
CXXOBJECTS:=$(patsubst %.cpp, %.o.$(OBJ_SUFFIX), $(CXXSOURCES))
SSOURCES:=$(shell find . -name *.s)
SOBJECTS:=$(patsubst %.s, %.o.$(OBJ_SUFFIX), $(SSOURCES))

prep: ;

build: $(COBJECTS) $(CXXOBJECTS) $(SOBJECTS)
%o.$(OBJ_SUFFIX): %c
	@echo Compiling $<
	@$(Clang) $(CFLAGS) -o $@ -c $<