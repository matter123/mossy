#! /usr/bin/env python3
import os
import sys
import sqlite3
import shutil

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

argc = len(sys.argv)
if argc == 2 and sys.argv[1] == "clean":
    shutil.rmtree('objs', ignore_errors=True)

print("checking source files")
conn = sqlite3.connect('.build/main.db')
