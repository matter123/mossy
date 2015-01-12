import sqlite3
import util
import message
import module


class arch(object):
    def __init__(self, reset):
        self.db = self.get_db_name()
        if self.db is not None and reset is not True:
            self.db = sqlite3.connect('db/' + self.db)

    def check_if_build(self, main_db):
        self.clean = []
        dirty = self.get_dirty_files()
        modules = []
        for file in dirty:
            mod = module.get_module(file)
            if not mod in modules:
                modules.append(mod)
        for mod in modules:
            if not mod.is_prepped():
                mod.prep()
        processed = []
        while len(dirty) > 0:
            file = dirty.pop(0)
            if file in processed:
                continue
            message.info(file + ' is dirty')
            processed.append(file)
            dirty = dirty + self.get_depends(file)

        return len(self.clean) > 0

    def do_build(self):
        pass

    #following are implemented by subclass
    def clean_file(self, file, from_ext):
        return False

    def get_dirty_files():
        return []

    def get_depends():
        return []

    def get_db_name(self):
        return None
