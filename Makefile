all: build run

build:
	#tools/gen_build_info.py
	$(MAKE) -f Makefile32 build
	$(MAKE) -f Makefile64 build
	#tools/gen_build_info.py delete

run:
	@grub2-mkrescue -o bootable.iso iso -- -zisofs level=6
	@~/bochs-2.6.6/bochs -f bochs.rc -q
