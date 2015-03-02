import sys

cpp_opt = '-O3 -DDEBUG'
c_opt = '-O3 -DDEBUG'

for arg in sys.argv:
    if arg.startswith('--c++-opt='):
        cpp_opt = arg[len('--c++-opt='):]
    if arg.startswith('--c-opt='):
        c_opt = arg[len('--c-opt='):]
wflags = ' -Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wdeprecated'
wflags += ' -Wredundant-decls -Werror=parentheses -Wno-unused-parameter'
wflags += ' -Wno-sign-compare -Wno-implicit-exception-spec-mismatch -Wsystem-headers'
cpp_opt += ' -std=gnu++1y -DMOSSY -DTEST=0' + wflags + ' --sysroot=./sysroot '
c_opt += ' -std=c11 -DMOSSY -DTEST=0' + wflags + ' --sysroot=./sysroot'
cpp_opt += ' -include ./srcs/kernel/stdlib/global.h'
c_opt += ' -include ./srcs/kernel/stdlib/global.h'
for arg in sys.argv:
    if arg.startswith('--test'):
        cpp_opt += ' -DTEST=1'
        c_opt += ' -DTEST=1'


def get_global_compile_opt(c):
    if c:
        return c_opt
    return cpp_opt
