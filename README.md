mossy2
======
Mossy is a hobbiest operating system, for the purpose of teaching myself, C++, portability, and code standards.
Mossy2 is based off of, but shares little code from, [Mossy1](http://bitbucket.org/matter123/mossy). Mossy2 is mostly C++ with a little bit of intel syntax assembly for boot services,

Building
--------
Building The Mossy Operating System requires a few runtime dependencies on the host computer namely:
  * A Unix like enviorment
  * gcc cross-compiled for your target ([building a cross compiler](http://wiki.osdev.org/GCC_Cross-Compiler))
  * python (2 or 3) to create runtime dependencies
  * grub 1.99 or later, grub 1.98 and earlier have difficulty executing elf64 binaries need for X64
  * nasm 2.10 or later
