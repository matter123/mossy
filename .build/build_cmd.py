import message
import find_modules
import get_arches


def build(conn):
    modules = find_modules.get_modules(conn)
    arches = get_arches.get_arches()
    do_build = False
    for arch in arches:
        if arch.check_if_build(conn):
            do_build = True
    if do_build:
        for arch in arches:
            if not arch.do_build():
                message.error('build failed')
    else:
        message.info('all files clean no build needed')
