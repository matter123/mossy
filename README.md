mossy2
======
Mossy is a hobbyist operating system, for the purpose of teaching myself, C++, portability, and code standards.
Mossy2 is based off of, but shares little code from, [Mossy1](http://bitbucket.org/matter123/mossy). Mossy2 is mostly C++ with a little bit of intel syntax assembly for boot services,

Building
--------
Building The Mossy Operating System requires a few runtime dependencies on the host computer namely:
  * gcc cross-compiled for your target ([building a cross compiler](http://wiki.osdev.org/GCC_Cross-Compiler))
  	_or_ clang
  * python 3
  * grub 1.99 or later, grub 1.98 and earlier have difficulty executing elf64 binaries need for X64
  * nasm 2.10 or later
  * optionally PIL or Pillow if the font was modified  
If this is a your first time building from this particular source tree you need to run
`./build.py reset`  
To build all possible arches run `.build.py`, a particular arch can be built by running `./build.py <arch>`
