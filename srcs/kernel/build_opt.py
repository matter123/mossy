import module
import os
import util
import subprocess


class kernel_module(module.module):
    def add_compile_opt(self, compile_opt):
        compile_opt.append('-I./srcs/kernel/hal')
        compile_opt.append('-I./srcs/kernel/sys')
        compile_opt.append('-I./srcs/kernel/acpi')
        compile_opt.append('-I./srcs/kernel/stdlib/include')
        compile_opt.append('-I./srcs/kernel/arch')
        compile_opt.append('-I./srcs/kernel/vendor')

    def prep(self):
        self.set_prepped(True)

    def get_final(self):
        return 'kernel'

    def is_in_module(self, file):
        if self.get_final() in os.path.split(file)[1]:
            return True
        return super(kernel_module, self).is_in_module(file)

    def clean_file(self, file):
        while(1):
            pass
        objects = []
        if self.get_final() in os.path.split(file[0])[1]:
            for root,\
                dirs,\
                files in os.walk(os.path.join(util.get_mossy_path(),
                                              file[1] + '/unicode')):
                for ofile in files:
                    objects.append(os.path.join(root, ofile))
            old_cd = os.getcwd()
            os.chdir(util.get_mossy_path())
            opts = ['ar', 'r', file[0]]
            opts += objects
            comp = subprocess.Popen(opts)
            os.chdir(old_cd)
            stdout = comp.communicate()[0]
            return comp.returncode is 0
        return False


def get_class(cursor):
    return kernel_module('kernel', cursor)
