import os
import sys
import traceback

root_fol = os.path.abspath(os.path.join(os.path.abspath(__file__), '../..'))
show_exc = False
for arg in sys.argv:
    if arg == '--show-except':
        show_exc = True


def get_mossy_path():
    return root_fol


def get_db_name(file):
    old_cd = os.getcwd()
    os.chdir(root_fol)
    return os.path.abspath(file)[len(root_fol) + 1:]
    os.chdir(old_cd)


def print_exc():
    if show_exc:
        traceback.print_exc()
