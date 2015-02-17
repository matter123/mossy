import arch
import tools.check_file
import util
import sys
import fnmatch
import os

skip_check = False
for arg in sys.argv:
    if arg == '--skip-file-check':
        skip_check = True


class check_source(arch.arch):
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

    def update_mtime(self, file):
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        mtime = os.stat(os.path.join(util.get_mossy_path(), dbfile)).st_mtime
        cur.execute('UPDATE files SET mtime = ? WHERE file = ?',
                    (mtime, file[0]))
        if not cur.rowcount:
            cur.execute('INSERT INTO files (mtime, file) VALUES (?,?)',
                        (mtime, file[0]))
        self.db_dirty = True

    def get_dirty_files(self):
        cur = self.db.cursor()
        dirty = []
        for root, dirs, files in os.walk(os.path.join(util.get_mossy_path(),
                                                      'srcs')):
            for file in (fnmatch.filter(files, '*.cpp') +
                         fnmatch.filter(files, '*.c') +
                         fnmatch.filter(files, '*.h') +
                         fnmatch.filter(files, '*.hpp')):
                file = util.get_db_name(os.path.join(root, file))
                if self.check_mtime(cur, file):
                    dirty.append((file, ))
        return dirty

    def get_depends(self, file):
        return []

    def clean_file(self, file):
        if not skip_check and not tools.check_file.check_file(file[0]):
            return False
        self.update_mtime(file)
        return True


def get_db_name():
    return 'check_soure.db'


def get_class(db):
    return check_source(db)
