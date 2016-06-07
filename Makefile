all: run
.PHONY: all headers libs kernel iso clean build_info run

-include env_info

build_info:
	@python3 gen_buildinfo.py $(CPP)
	@touch build_info
-include build_info

export VERBOSE ?=0
ifeq ($(VERBOSE), 1)
	export V=
else
	export V=@
endif
clean:
	$(MAKE) -C src/kernel clean
	$(MAKE) -C src/libc clean
	$(MAKE) -C src/libkpp clean
	@-rm mossy.iso 2> /dev/null

cleanall: clean
	$(MAKE) -C src/libacpica clean

headers:
	$(MAKE) -C src/kernel headers
	$(MAKE) -C src/libc headers
	$(MAKE) -C src/libkpp headers
	$(MAKE) -C src/libacpica headers

sysroot/usr/lib/libk: libs
sysroot/usr/lib/libkpp: libs
sysroot/usr/lib/libacpica: libs
libs: headers
	$(MAKE) -C src/libc
	$(MAKE) -C src/libkpp
	$(MAKE) -C src/libacpica

kernel: sysroot/boot/kernel
sysroot/boot/kernel: sysroot/usr/lib/libk sysroot/usr/lib/libkpp sysroot/usr/lib/libacpica
	$(MAKE) -C src/kernel

iso: mossy.iso
mossy.iso: sysroot/boot/kernel sysroot/boot/grub/grub.cfg
	grub2-mkrescue --compress=gz -o mossy.iso sysroot || grub-mkrescue --compress=gz -o mossy.iso sysroot

run: iso
	~/src/bochs/bochs -f bochs.rc -q
