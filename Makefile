all: build run

build: x86-build x64-build

run:
	@grub2-mkrescue -o bootable.iso iso -- -zisofs level=6
	@~/bochs-2.6.2/bochs -f bochs.rc -q

x86-all: x86-build run

x64-all: x64-build run

x86-build:
	$(MAKE) -f Makefile32 build

x64-build:
	$(MAKE) -f Makefile64 build