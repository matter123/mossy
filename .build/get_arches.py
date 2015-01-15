import os
import util
import importlib
import message
import sqlite3

arches = []


def get_arches():
    global arches
    old_cd = os.getcwd()
    arches_p = os.path.join(util.get_mossy_path(), '.build/arches')
    os.chdir(os.path.join(util.get_mossy_path(), '.build'))
    for name in os.listdir(arches_p):
        if os.path.isfile(os.path.join(arches_p, name)):
            try:
                arch = importlib.import_module('arches.' + name[:-3])
                db = sqlite3.connect('db/' + arch.get_db_name())
                arches.append(arch.get_class(db))
            except:
                message.warning('import of arch ' + name + ' failed')
                util.print_exc()
    os.chdir(old_cd)
    return arches


def get_db_names():
    db = []
    old_cd = os.getcwd()
    arches_p = os.path.join(util.get_mossy_path(), '.build/arches')
    os.chdir(os.path.join(util.get_mossy_path(), '.build'))
    for name in os.listdir(arches_p):
        if os.path.isfile(os.path.join(arches_p, name)):
            try:
                arch = importlib.import_module('arches.' + name[:-3])
                db.append('db/' + arch.get_db_name())
            except:
                message.warning('import of arch ' + name + ' failed')
                util.print_exc()
    os.chdir(old_cd)
    return db
