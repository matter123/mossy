import message
import find_modules
import get_arches
import sys
import tools.build_info
import util
import subprocess


def build(conn, build_arg):
    if len(sys.argv) - 1 is build_arg:
        arches = get_arches.get_arches()
    else:
        arches = []
        for arg in sys.argv[build_arg:]:
            if arg[0] is not '-':
                arches.append(get_arches.get_arch(arg))
    find_modules.get_modules(conn)
    do_build = False
    for arch in arches:
        if arch.check_if_build(conn):
            do_build = True
    if do_build:
        message.info('some arches are dirty, building')
        tools.build_info.gen(util.get_mossy_path())
        for arch in arches:
            if not arch.do_build():
                for arch in arches:
                    arch.save_db()
                tools.build_info.delete(util.get_mossy_path())
                message.error('build failed')
                sys.exit(1)
            arch.save_db()
        try:
            tools.build_info.delete(util.get_mossy_path())
        except:
            pass
        subprocess.call(['grub2-mkrescue', '--compress=gz', '-o', 'bootable.iso', 'sysroot'])
    else:
        message.info('all files are clean, no build needed')
