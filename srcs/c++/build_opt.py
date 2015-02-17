import lib_build_opt


def get_class(cursor):
    return lib_build_opt.module('c++', cursor)
