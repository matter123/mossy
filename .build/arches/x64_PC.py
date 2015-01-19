import os
import sys
import fnmatch
import util
import subprocess
import arch
#import tools.s_parse as s_parse
import compile_opt
import find_modules

compiler_cpp = 'clang++ --target=x86_64-elf -Wdocumentation'
compiler_c = 'clang --target=x86_64-elf -Wdocumentation'
compiler_opt = '-fno-omit-frame-pointer -fno-stack-protector'
for arg in sys.argv:
    if arg.startswith('--c++-x64_PC='):
        compiler_cpp = arg[len('--c++-x64_PC='):]
    if arg.startswith('--c-x64_PC='):
        compiler_c = arg[len('--c-x64_PC='):]
    if arg.startswith('--x64_PC-opt='):
        compiler_opt = arg[len('--x64_PC-opt='):]
compiler_opt += ' -ffreestanding -fno-common -fno-rtti -fno-exceptions'
compiler_opt += ' -mcmodel=kernel -mno-red-zone -mno-3dnow -DVENDOR=_PC'
compiler_opt += ' -DARCH=_x64'


def run_compiler(c, opts):
    old_cd = os.getcwd()
    os.chdir(util.get_mossy_path())
    if c:
        opts = (compiler_c + ' ' + opts.strip()).split(' ')
    else:
        opts = (compiler_cpp + ' ' + opts.strip()).split(' ')
    comp = subprocess.Popen(opts, stdout=subprocess.PIPE)
    stdout = comp.communicate()[0]
    os.chdir(old_cd)
    return (comp.returncode is 0, stdout)


class x64_pc(arch.arch):
    def get_compile_opts(self, c, file):
        opt = compile_opt.get_global_compile_opt(c).strip()
        opt += compiler_opt.strip()
        opt = opt.split(' ')
        find_modules.get_module(file).add_compile_opt(opt)
        return ' '.join(opt)

    def update_depend(self, cur, file):
        depends = []
        ext = os.path.splitext(file)[1]
        if ext == '.s':
            pass  # depends = s_parse.get_depends(file)
        elif ext == '.c':
            out = run_compiler(True, '-MM -MG -MP ' +
                               self.get_compile_opts(True, file) + ' ' + file)
            if out[0]:
                print(out[1])
        elif ext == '.cpp':
            out = run_compiler(False, '-MM -MG -MP ' +
                               self.get_compile_opts(False, file) + ' ' + file)
            if out[0]:
                print(out[1])

    def check_mtime(self, cur, file):
        if not os.path.isfile(file):
            return True
        dbfile = util.get_db_name(file)
        # check mtime in db using dbfile compare to os.stat
        # if not exist in db or stat mtime is newer return True
        cur.execute('SELECT mtime FROM files WHERE file = ?',
                    (dbfile,))
        res = cur.fetchall()
        if len(res):
            if res[0][0] < os.stat(file).st_mtime:
                return True
            return False
        return True

    def get_objfile(self, file):
        fname = os.path.splitext(file.replace('srcs/', 'objs/x64_PC/', 1))[0]
        return fname + '.o'

    def update_mtime(self, file):
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        mtime = os.stat(os.path.join(util.get_mossy_path(), dbfile)).st_mtime
        cur.execute('UPDATE files SET mtime = ? WHERE file = ?',
                    (mtime, file[0]))
        if not cur.rowcount:
            cur.execute('INSERT INTO files (mtime, file) VALUES (?,?)',
                        (mtime, file[0]))
        self.db.commit()

    def get_dirty_files(self):
        cur = self.db.cursor()
        dirty = []
        for root, dirs, files in os.walk(os.path.join(util.get_mossy_path(),
                                                      'srcs')):
            for file in fnmatch.filter(files, '*.h'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))

            for file in fnmatch.filter(files, '*.cpp'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
                # get resultant o file
                ofile = self.get_objfile(file)
                if self.check_mtime(cur, ofile):
                    dirty.append((util.get_db_name(ofile), file))

            for file in fnmatch.filter(files, '*.c'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
                # get resultant o file
                ofile = self.get_objfile(file)
                if self.check_mtime(cur, ofile):
                    dirty.append((util.get_db_name(ofile), file))

            for file in fnmatch.filter(files, '*.s'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    self.update_depend(cur, file)
                    dirty.append((util.get_db_name(file), ))
                # get resultant o file
                ofile = self.get_objfile(file)
                if self.check_mtime(cur, ofile):
                    self.update_depend(cur, file)
                    dirty.append((util.get_db_name(ofile), file))

            for file in fnmatch.filter(files, '*.inc'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
        return dirty

    def get_depends(self, file):
        if file[0].endswith('.c') or file[0].endswith('.cpp') or\
                file[0].endswith('.s'):
            self.update_depend(file[0])
            return (self.get_objfile(file[0]), file[0])
        if file[0].endswith('.o'):
            return (find_modules.get_module(file[1]).get_final(),)


def get_db_name():
    return 'x64_PC.db'


def get_class(db):
    return x64_pc(db)
