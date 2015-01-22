import os
import shutil
import module
import util


class unicode_module(module.module):
    def prep(self):
        mossy_main = util.get_mossy_path()
        inc = os.path.abspath(os.path.join(mossy_main,
                                           './srcs/unicode/include'))
        files = [f for f in os.listdir(inc)
                 if os.path.isfile(os.path.join(inc, f))]
        install_path = os.path.abspath(os.path.join(mossy_main,
                                                    './sysroot/usr/include'))
        for file in files:
            shutil.copyfile(os.path.join(inc, file),
                            os.path.join(install_path, file))
        self.set_prepped(True)

        def add_compile_opt(self, compile_opt):
            compile_opt.append('-I./srcs/unicode/include')

        def get_final(self):
            return 'libunicode.a'


def get_class(cursor):
    return unicode_module('unicode', cursor)
