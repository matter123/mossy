import os
import sys
import fnmatch
import util
import subprocess
import arch
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
    return (comp.returncode is 0, stdout.decode('unicode_escape'))


class x64_pc(arch.arch):
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
        self.db.commit()

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
            for file in (fnmatch.filter(files, '*.cpp') +
                         fnmatch.filter(files, '*.c') +
                         fnmatch.filter(files, '*.s')):
                # .cpp, .c and .s files make .o files
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
                # get resultant o file
                ofile = self.get_objfile(file)
                if self.check_mtime(cur, ofile):
                    dirty.append((util.get_db_name(ofile), file))

            for file in (fnmatch.filter(files, '*.h') +
                         fnmatch.filter(files, '*.hpp') +
                         fnmatch.filter(files, '*.inc')):
                # .h and .inc files do not make .o files
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
        return dirty

    def get_depends(self, file):
        if file[0].endswith('.c') or file[0].endswith('.cpp') or\
                file[0].endswith('.s'):
            self.update_depend(file[0])
            return [(self.get_objfile(file[0]), file[0])]
        if file[0].endswith('.o'):
            return [('objs/x64_PC/' +
                     find_modules.get_module(file[1]).get_final(),)]
        if file[0].endswith('.h'):
            if find_modules.get_module(file[0]).name is not 'kernel' and\
                    'include' in file[0]:
                return [('sysroot/usr/include/' + os.path.basename(file[0]),)]
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        cur.execute('SELECT file FROM depends WHERE depend = ?',
                    (dbfile, ))
        res = []
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
            if ext == '.c' or ext == '.cpp' or ext == '.s':
                sources.append(file)
            if ext == '.o':
                objects.append(file)
            if ext == '.a':
                archives.append(file)
            if ext == '':
                kernels.append(file)
        self.clean = headers + sources + objects + archives + kernels

    def clean_file(self, file):
        ext = os.path.splitext(file[0])[1]
        if ext == '.h' or ext == '.hpp' or ext == '.inc' or\
                ext == '.c' or ext == '.cpp' or ext == '.s':
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
                pass  # skipping nasm for now
            if out[0]:
                self.update_mtime(file)
                return True
            return False

        if ext == '.a':
            return False
        if ext == '':
            return False
        return True


def get_db_name():
    return 'x64_PC.db'


def get_class(db):
    return x64_pc(db)
