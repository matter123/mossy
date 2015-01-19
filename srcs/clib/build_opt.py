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
        try:
            os.makedirs(os.path.join(install_path, 'clib'))
        except:
            pass
        try:
            os.makedirs(os.path.join(install_path, 'klib'))
        except:
            pass
        for file in files:
            shutil.copyfile(os.path.join(inc, file),
                            os.path.join(install_path, 'clib/' + file))
            shutil.copyfile(os.path.join(inc, file),
                            os.path.join(install_path, 'klib/' + file))
        self.set_prepped(True)

        def add_compile_opt(self, compile_opt):
            compile_opt.append('-I./srcs/clib/include')


def get_class(cursor):
    return clib_module('clib', cursor)
