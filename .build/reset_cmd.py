import sqlite3
import message
import shutil
import util
import os
import get_arches


def reset():
    message.info('resetting build state')
    shutil.rmtree(os.path.join(util.get_mossy_path(), '.build/db'),
                  ignore_errors=True)
    os.makedirs(os.path.join(util.get_mossy_path(), '.build/db'))
    shutil.rmtree(os.path.join(util.get_mossy_path(), 'objs'),
                  ignore_errors=True)
    os.makedirs(os.path.join(util.get_mossy_path(), 'objs'))
    main = sqlite3.connect('db/main.db')
    cur = main.cursor()
    cur.execute('CREATE TABLE modules (name text, prep int)')
    cur.execute('''INSERT INTO modules VALUES('None', 1)''')
    main.commit()
    main.close()
    for db in get_arches.get_db_names():
        db = sqlite3.connect(db)
        cur = db.cursor()
        cur.execute('CREATE TABLE files (file text, mtime int)')
        cur.execute('CREATE TABLE depends (file text, depend, text)')
        db.commit()
        db.close()
