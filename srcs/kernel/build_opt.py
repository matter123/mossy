import module
import os
import util
import compile_opt


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
        if os.path.splitext(self.get_final())[0] in os.path.split(file)[1]:
            return True
        return super(kernel_module, self).is_in_module(file)

    def clean_file(self, file):
        if not os.path.splitext(self.get_final())[0] in\
                os.path.split(file[0])[1]:
            return False
        objects = []
        linker_options = ['-static', '-ffreestanding', '-nostdlib',
                          '-Wl,-z,max-page-size=0x1000']
        for root,\
            dirs,\
            files in os.walk(os.path.join(util.get_mossy_path(),
                                          file[1] + '/kernel')):
            for ofile in files:
                objects.append(os.path.join(root, ofile))
        old_cd = os.getcwd()
        linker_options.append('-Tsrcs/kernel/link.ld.' + file[2])
        linker_options.append('-o')
        linker_options.append(file[0])
        linker_options += objects
        linker_options.append('-lk' + file[2])
        linker_options.append('-lunicode' + file[2])
        linker_options.append('-lgcc')
        os.chdir(util.get_mossy_path())
        res = file[3](False, (compile_opt.get_global_compile_opt(False)
                              .strip() + ' ' + ' '.join(linker_options))
                      .replace('--sysroot=./sysroot',
                      '-L./sysroot/usr/lib'))
        os.chdir(old_cd)
        return res[0]


def get_class(cursor):
    return kernel_module('kernel', cursor)
