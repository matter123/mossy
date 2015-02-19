#!usr/bin/env python3
import sys
import os
sys.dont_write_bytecode = False
import message
import build_cmd
import reset_cmd
import find_modules
import get_arches
import sqlite3


def go(argv):
    os.chdir(os.path.dirname(__file__))
    conn = sqlite3.connect('db/main.db')
    command = 'build'
    next = -1
    if len(argv) > 1:
        next = 1
        while next < len(argv) and argv[next].startswith('-'):
            next += 1
        if next < len(argv):
            command = argv[next]
        else:
            next = len(argv) - 1
    if next is -1:
        next = 0
    if command == "build":
        message.info("building")
        build_cmd.build(conn, next)
    if command == "reset":
        reset_cmd.reset()
