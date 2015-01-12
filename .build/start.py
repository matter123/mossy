#!usr/bin/env python3
import sys
import os
sys.dont_write_bytecode = False
import message
import build_cmd
import find_modules
import sqlite3


def go(argv):
    os.chdir(os.path.dirname(__file__))
    conn = sqlite3.connect('db/main.db')
    command = 'build'
    if len(argv) > 1:
        next = 1
        while (next + 1) < len(argv) and argv[next].startswith('-'):
            next += 1
        if (next + 1) < len(argv):
            command = argv[next]
    if command == "build":
        message.info("building")
    message.info(command)
    print(find_modules.get_modules(conn))
