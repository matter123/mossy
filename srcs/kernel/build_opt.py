import module


class kernel_module(module.module):
    def add_compile_opt(self, compile_opt):
        compile_opt.append('-I./srcs/kernel/hal')
        compile_opt.append('-I./srcs/kernel/sys')
        compile_opt.append('-I./srcs/kernel/stdlib/include')
        compile_opt.append('-include')
        compile_opt.append('./srcs/kernel/stdlib/global.h')
        compile_opt.append('-I./srcs/kernel/arch')
        compile_opt.append('-I./srcs/kernel/vendor')

    def prep(self):
        self.set_prepped(True)

    def get_final(self):
        return 'kernel'


def get_class(cursor):
    return kernel_module('kernel', cursor)
