import message
import find_modules
import get_arches
import sys


def build(conn):
    find_modules.get_modules(conn)
    arches = get_arches.get_arches()
    do_build = False
    for arch in arches:
        if arch.check_if_build(conn):
            do_build = True
    if do_build:
        message.info('some arches are dirty, building')
        for arch in arches:
            if not arch.do_build():
                message.error('build failed')
                sys.exit(1)
    else:
        message.info('all files are clean, no build needed')
