import os
import util
import importlib
import message
import module

modules = []


def get_modules(db):
    global modules
    old_cd = os.getcwd()
    os.chdir(util.get_mossy_path())
    srcs = os.path.join(util.get_mossy_path(), 'srcs')
    mods = []
    for name in os.listdir(srcs):
        if os.path.isdir(os.path.join(srcs, name)):
            if os.path.isfile(os.path.join(srcs, name + '/build_opt.py')):
                try:
                    mods.append(importlib.import_module('srcs.' +
                                                        name +
                                                        '.build_opt')
                                .get_class(db.cursor()))
                except:
                    message.warning('import of module ' + name + ' failed')
                    util.print_exc()
    os.chdir(old_cd)
    mods.append(module.module('None', db.cursor()))
    modules = mods
    return mods


def get_module(file):
    for mod in modules:
        if mod.is_in_module(file):
            return mod
