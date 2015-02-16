import os
import sys
import fnmatch
import util
import subprocess
import arch
import compile_opt
import find_modules
import tools.check_file

compiler_cpp = 'clang++ --target=i686-elf -Wdocumentation'
compiler_c = 'clang --target=i686-elf -Wdocumentation'
compiler_opt = '-fno-omit-frame-pointer -fno-stack-protector'
skip_check = False
for arg in sys.argv:
    if arg.startswith('--c++-x86_PC='):
        compiler_cpp = arg[len('--c++-x86_PC='):]
    if arg.startswith('--c-x86_PC='):
        compiler_c = arg[len('--c-x86_PC='):]
    if arg.startswith('--x86_PC-opt='):
        compiler_opt = arg[len('--x86_PC-opt='):]
    if arg == '--skip-file-check':
        skip_check = True
compiler_opt += ' -ffreestanding -fno-common -fno-rtti -fno-exceptions'
compiler_opt += ' -mno-3dnow -DVENDOR=_PC'
compiler_opt += ' -DARCH=_x86'


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
    return (comp.returncode is 0, stdout.decode('unicode_escape'))


class x86_pc(arch.arch):
    def run_nasm(self, file):
        old_cd = os.getcwd()
        os.chdir(util.get_mossy_path())
        temp = file[0].replace('.o', '.sp')
        run_compiler(False, '-DASM -E -P -o ' + temp + ' -x c++ ' +
                     self.get_compile_opts(False, file[1]) + ' ' + file[1])
        comp = subprocess.Popen(['nasm', '-felf', '-o',
                                 file[0], temp])
        comp.communicate()
        os.remove(temp)
        os.chdir(old_cd)
        return (comp.returncode is 0,)

    def get_compile_opts(self, c, file):
        opt = compile_opt.get_global_compile_opt(c).strip()
        opt += ' ' + compiler_opt.strip()
        opt = opt.split(' ')
        find_modules.get_module(file).add_compile_opt(opt)
        return ' '.join(opt)

    def update_depend(self, file):
        depends = []
        ext = os.path.splitext(file)[1]
        cur = self.db.cursor()
        out = None
        if ext == '.s':
            with open(file, 'r') as f:
                for line in f:
                    if line.startswith('%include'):
                        depends.append(line.split(' ')[1][1:-1])
        elif ext == '.c':
            out = run_compiler(True, '-M -MG ' +
                               self.get_compile_opts(True, file) + ' ' + file)
        elif ext == '.cpp':
            out = run_compiler(False, '-M -MG ' +
                               self.get_compile_opts(False, file) + ' ' + file)
        if out is not None and out[0]:
            depends = [f.strip() for f in out[1].split()[2:] if 'bin' not in f]
        cur.execute('DELETE FROM depends WHERE file = ?', (file,))
        for depend in depends:
            cur.execute('INSERT INTO depends (file, depend) VALUES (?, ?)',
                        (file, depend))
        self.db_dirty = True

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
        fname = os.path.splitext(file.replace('srcs/', 'objs/x86_PC/', 1))[0]
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

    def get_dirty_files(self):
        cur = self.db.cursor()
        dirty = []
        for root, dirs, files in os.walk(os.path.join(util.get_mossy_path(),
                                                      'srcs')):
            for file in (fnmatch.filter(files, '*.cpp') +
                         fnmatch.filter(files, '*.c') +
                         fnmatch.filter(files, '*.s')):
                # .cpp, .c and .s files make .o files
                file = util.get_db_name(os.path.join(root, file))
                if self.check_mtime(cur, file):
                    dirty.append((file, ))
                # get resultant o file
                ofile = util.get_db_name(self.get_objfile(file))
                if self.check_mtime(cur, ofile):
                    dirty.append((ofile, file))

            for file in (fnmatch.filter(files, '*.h') +
                         fnmatch.filter(files, '*.hpp') +
                         fnmatch.filter(files, '*.inc')):
                # .h and .inc files do not make .o files
                file = util.get_db_name(os.path.join(root, file))
                if self.check_mtime(cur, file):
                    dirty.append((file, ))
        return dirty

    def get_depends(self, file):
        if file[0].endswith('.c') or file[0].endswith('.cpp') or\
                file[0].endswith('.s'):
            self.update_depend(file[0])
            return [(self.get_objfile(file[0]), file[0])]
        if file[0].endswith('.o'):
            return [('sysroot/usr/lib/' +
                     os.path.splitext(find_modules.get_module(file[1])
                                      .get_final())[0] +
                    'x86_PC' + os.path.splitext(find_modules
                                                .get_module(file[1])
                                                .get_final())[1],
                     'objs/x86_PC', 'x86_PC', run_compiler)]
        res = []
        if file[0].endswith('.h'):
            if find_modules.get_module(file[0]).name is not 'kernel' and\
                    'include' in file[0]:
                res.append(('sysroot/usr/include/' +
                            os.path.basename(file[0]),))
        if self.db_dirty:
            self.db.commit()
            self.db_dirty = False
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        cur.execute('SELECT file FROM depends WHERE depend = ?',
                    (dbfile, ))
        for row in cur:
            res.append((row[0],))
        return res

    def fix_build_order(self):
        headers = []
        sources = []
        objects = []
        archives = []
        kernels = []
        for file in self.clean:
            ext = os.path.splitext(file[0])[1]
            if ext == '.h' or ext == '.hpp' or ext == '.inc':
                headers.append(file)
            elif ext == '.c' or ext == '.cpp' or ext == '.s':
                sources.append(file)
            elif ext == '.o':
                objects.append(file)
            elif ext == '.a':
                archives.append(file)
            elif ext == '':
                kernels.append(file)
        self.clean = headers + sources + objects + archives + kernels

    def clean_file(self, file):
        ext = os.path.splitext(file[0])[1]
        if ext == '.h' or ext == '.hpp' or ext == '.c' or\
                ext == '.cpp':
            if skip_check and not tools.check_file.check_file(file[0]):
                return False
            self.update_mtime(file)
            return True
        if ext == '.s' or ext == '.inc':
            self.update_mtime(file)
            return True
        if ext == '.o':
            os.makedirs(os.path.dirname(file[0]), exist_ok=True)
            ext = os.path.splitext(file[1])[1]
            if ext == '.c':
                out = run_compiler(True, '-c -o ' + file[0] + ' ' +
                                   self.get_compile_opts(True, file[1]) +
                                   ' ' + file[1])
            if ext == '.cpp':
                out = run_compiler(False, '-c -o ' + file[0] + ' ' +
                                   self.get_compile_opts(False, file[1]) +
                                   ' ' + file[1])
            if ext == '.s':
                out = self.run_nasm(file)

            if out[0]:
                self.update_mtime(file)
                return True
            return False

        if ext == '.a':
            return False
        if ext == '':
            return False
        return True

    def get_name(self):
        return 'x86_PC'

    def get_standard():
        return True


def get_db_name():
    return 'x86_PC.db'


def get_class(db):
    return x86_pc(db)
