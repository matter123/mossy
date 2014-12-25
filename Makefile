all: build
WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated \
-Wredundant-decls -Werror=parentheses
DFLAGS?=-DDEBUG -DTEST=1
DFLAGS+=-DMOSSY
WFLAGSOFF=-Wno-unused-parameter -Wno-sign-compare -Wno-implicit-exception-spec-mismatch
IFLAGS=--sysroot=./sysroot
CFLAGS?=-O3
CXXFLAGS?=-O3
CFLAGS+=-std=c11 $(WFLAGSON) $(WFLAGSON) $(DFLAGS) $(IFLAGS)
CXXFLAGS+=-std=gnu++1y $(WFLAGSON) $(WFLAGSON) $(DFLAGS) $(IFLAGS)

build:
	#build_files/tools/gen_build_info.py
	$(MAKE) -f Makefile86_PC prep
	$(MAKE) -f Makefile64_PC prep
	$(MAKE) -f Makefile86_PC build
	$(MAKE) -f Makefile64_PC build
	#build_files/tools/gen_build_info.py delete

buildiso:
	@grub2-mkrescue --compress=gz -o bootable.iso iso

run:
	@bochs -f bochs.cfg -q
clean:
	$(MAKE) -f Makefile86_PC clean
	$(MAKE) -f Makefile64_PC clean
