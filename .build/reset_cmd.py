import sqlite3
import message
import shutil
import util
import os


def reset():
    message.info('resetting build state')
    shutil.rmtree(os.path.join(util.get_mossy_path(), '.build/db'),
                  ignore_errors=True)
    os.makedirs(os.path.join(util.get_mossy_path(), '.build/db'))
    main = sqlite3.connect('db/main.db')
    cur = main.cursor()
    cur.execute('DROP TABLE IF EXISTS modules')
    main.commit()
