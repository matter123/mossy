
def install_headers(mossy_main):
    pass


def get_addl_compile_opt():
    return ['-I./srcs/kernel/hal', '-I./srcs/kernel/sys',
            '-I./srcs/kernel/stdlib/include',
            '-i./srcs/kernel/stdlib/global.h',
            '-I./srcs/kernel/arch', '-I./srcs/kernel/vendor',
            '-I/usr/include/klib']
