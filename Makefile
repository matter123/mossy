all: iso
.PHONY: all headers libs kernel iso clean
clean:
	$(MAKE) -C src/kernel clean
	$(MAKE) -C src/libc clean
	@-rm mossy.iso 2> /dev/null
headers:
	$(MAKE) -C src/kernel headers
	$(MAKE) -C src/libc headers

sysroot/usr/lib/libk: libs
libs: headers
	$(MAKE) -C src/libc

kernel: sysroot/boot/kernel
sysroot/boot/kernel: sysroot/usr/lib/libk
	$(MAKE) -C src/kernel

iso: mossy.iso
mossy.iso: sysroot/boot/kernel sysroot/boot/grub/grub.cfg
	grub2-mkrescue --compress=gz -o mossy.iso sysroot
