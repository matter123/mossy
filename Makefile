all: build run
export
WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated \
-Wredundant-decls -Werror=parentheses
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-gnu
IFLAGS=-Isrc/stdlib/include -Ilibs/clib/include -Ilibs/encode/include -Isrc/hal -Isrc/arch \
-Isrc/vendor -Isrc/sys -include src/stdlib/global.h

build:
	tools/gen_build_info.py
	$(MAKE) -f Makefile32 build
	#$(MAKE) -f Makefile64 build
	tools/gen_build_info.py delete

run:
	@grub2-mkrescue -o bootable.iso iso -- -zisofs level=6
	@~/bochs-2.6.6/bochs -f bochs.cfg -q
	#@scripts/run_emulator64 `pwd`
