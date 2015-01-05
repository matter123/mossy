import os
import shutil


def install_headers(mossy_main):
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


def get_addl_compile_opt():
    return ['-I./srcs/clib/include']
