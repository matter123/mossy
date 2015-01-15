import os
import sys
import fnmatch
import util
import subprocess
import arch

compiler_cpp = 'clang++ --target=x86_64-elf -Wdocumentation'
compiler_c = 'clang --target=x86_64-elf -Wdocumentation'
for arg in sys.argv:
    if arg.startswith('--c++-x64_PC='):
        compiler_cpp = arg[len('--c++-x64_PC='):]
    if arg.startswith('--c-x64_PC='):
        compiler_c = arg[len('--c-x64_PC='):]


def run_compiler(c, opts):
    old_cd = os.getcwd()
    os.chdir(util.get_mossy_path())
    if c:
        opts = (compiler_c + opts).split(' ')
    else:
        opts = (compiler_cpp + opts).split(' ')
    res = subprocess.call(opts)
    os.chdir(old_cd)
    return res is 0


class x64_pc(arch.arch):
    pass
