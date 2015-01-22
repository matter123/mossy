#!/usr/bin/env python3
import sys
import os.path as path
if sys.hexversion < 0x03000000:
    print("python 2 is not supported")
    exit(1)
sys.path.append(
    path.normpath(path.join(path.dirname(path.realpath(__file__)),
                            "./.build")))
sys.dont_write_bytecode = True
import start
start.go(sys.argv)
