import lib_build_opt


class test(lib_build_opt.module):
    def add_compile_opt(self, compile_opt):
        super(test, self).add_compile_opt(compile_opt)
        compile_opt.append('-I./srcs/kernel/hal')
        compile_opt.append('-I./srcs/kernel/arch')
        compile_opt.append('-I./srcs/kernel/stdlib/include')


def get_class(cursor):
    return test('test', cursor)
