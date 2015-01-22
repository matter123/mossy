import message
import find_modules


class arch(object):
    def __init__(self, db):
        self.db = db

    def check_if_build(self, main_db):
        self.clean = []
        dirty = self.get_dirty_files()
        modules = []
        for file in dirty:
            mod = find_modules.get_module(file[0])
            if mod not in modules:
                modules.append(mod)
        for mod in modules:
            if not mod.is_prepped():
                mod.prep()
        processed = []
        while len(dirty) > 0:
            file = dirty.pop(0)
            if file in processed:
                continue
            message.info(file[0] + ' is dirty')
            processed.append(file)
            dirty = dirty + self.get_depends(file)
            self.clean.append(file)
        return len(self.clean) > 0

    def do_build(self):
        if len(self.clean) == 0:
            return
        self.fix_build_order()
        while len(self.clean) > 0:
            file = self.clean.pop(0)
            message.info('building ' + file[0])
            if not self.clean_file(file):
                mod = find_modules.get_module(file[0])
                if not mod.clean_file(file):
                    message.error(file[0] + ' has no valid cleaning target')
                    return False
        return True

    # following are implemented by subclass
    def clean_file(self, file):
        return False

    def get_dirty_files(self):
        return []

    def get_depends(self, file):
        return []

    def fix_build_order(self):
        pass
