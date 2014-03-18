all: x86-all x64-all

build: x86-build x64-build

run: x86-run x64-run

x86-all: x86-build x86-run

x64-all: x64-build x64-run

x86-build:
	make -f Makefile32 build

x64-build:
	make -f Makefile64 build

x86-run:
	make -f Makefile32 test

x64-run:
	make -f Makefile64 test
