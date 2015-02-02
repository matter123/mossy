import message
import find_modules
import get_arches
import sys
import tools.build_info
import util
import subprocess


def build(conn, build_arg):
    find_modules.get_modules(conn)
    arches = get_arches.get_arches()
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
        subprocess.Popen(['grub2-mkrescue', '--compress=gz', '-o', 'bootable.iso', 'sysroot'])
    else:
        message.info('all files are clean, no build needed')
