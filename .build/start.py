#!usr/bin/env python3
import sys
import os
sys.dont_write_bytecode = False
import message
import sqlite3
import build_cmd


def go(argv):
    os.chdir(os.path.dirname(__file__))
    command = 'build'
    if len(argv) > 1:
        command = argv[1]
    conn = sqlite3.connect('main.db')
    cursor = conn.cursor()
    if command == "reset":
        message.info('resetting build system')
        cursor.execute('DROP TABLE IF EXISTS files')
        cursor.execute('DROP TABLE IF EXISTS depends')
        cursor.execute('''CREATE TABLE files
                         (id INTEGER PRIMARY KEY ASC,name text unique,mod_date int)''')
        cursor.execute('''CREATE TABLE depends
                         (file int,depend int)''')
        conn.commit()
        conn.close()
        message.info("done")
        exit(1)
    if command == "build":
        message.info("building")
        build_cmd.build(conn, cursor)
    message.info(command)
