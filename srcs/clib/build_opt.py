import os
import shutil
import module
import util


class clib_module(module.module):
    def prep(self):
        mossy_main = util.get_mossy_path()
        inc = os.path.abspath(os.path.join(mossy_main, './srcs/clib/include'))
        files = [f for f in os.listdir(inc)
                 if os.path.isfile(os.path.join(inc, f))]
        install_path = os.path.abspath(os.path.join(mossy_main,
                                                    './sysroot/usr/include'))
        for file in files:
            shutil.copyfile(file, os.path.join(install_path, 'clib'))
            shutil.copyfile(file, os.path.join(install_path, 'klib'))

        for file in [f for f in os.listdir(os.path.join(inc, 'k'))
                     if os.path.isfile(os.path.join(inc, f))]:
            shutil.copyfile(file, os.path.join(install_path, 'klib'))
        for file in [f for f in os.listdir(os.path.join(inc, 'c'))
                     if os.path.isfile(os.path.join(inc, f))]:
            shutil.copyfile(file, os.path.join(install_path, 'clib'))
        self.set_prepped(True)

        def add_compile_opt(self, compile_opt):
            compile_opt.append('-I./srcs/clib/include')


def get_class(cursor):
    return clib_module('clib', cursor)
