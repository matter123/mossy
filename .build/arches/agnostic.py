import os
import fnmatch
import util
import xml.etree.ElementTree as ET
import subprocess
import message
import arch


class agnostic(arch.arch):
    def clean_file(self, file):
        if file[0].endswith('mbf'):
            fname = os.path.splitext(os.path.split(file[0])[1])[0]
            os.chdir(util.get_mossy_path())
            subprocess.call([os.executable,
                            os.path.join(mossy_fol,
                                         '.build/tools/make_mbf.py'),
                            file[1],
                            file[0]])
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        print(file[0])
        mtime = os.stat(os.path.join(util.get_mossy_path(), dbfile)).st_mtime
        cur.execute('UPDATE files SET mtime = ? WHERE file = ?',
                    (mtime, file[0]))
        if not cur.rowcount:
            cur.execute('INSERT INTO files (mtime, file) VALUES (?,?)',
                        (mtime, file[0]))
        self.db.commit()
        return True

    def check_mtime(self, cur, file):
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

    def update_depends(self, cur, file):
        dbfile = util.get_db_name(file)
        if file.endswith('xml'):
            tree = ET.parse(file)
            fol = os.path.split(os.path.abspath(file))[0]
            png = util.get_db_name(os.path.join(fol,
                                                tree.getroot().attrib["png"]))
            cur.execute('DELETE FROM depends WHERE file = ? ', (dbfile,))
            cur.execute('INSERT INTO depends (file, depend) VALUES (?,?)',
                        (dbfile, png))
            self.db.commit()

    def get_dirty_files(self):
        cur = self.db.cursor()
        dirty = []
        for root, dirs, files in os.walk(os.path.join(util.get_mossy_path(),
                                                      'srcs/fonts')):
            for file in fnmatch.filter(files, '*.xml'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
                    self.update_depends(cur, file)
            for file in fnmatch.filter(files, '*.png'):
                file = os.path.join(root, file)
                if self.check_mtime(cur, file):
                    dirty.append((util.get_db_name(file), ))
        return dirty

    def get_depends(self, file):
        cur = self.db.cursor()
        dbfile = util.get_db_name(file[0])
        cur.execute('SELECT file FROM depends WHERE depend = ?',
                    (dbfile, ))
        res = []
        for row in cur:
            res.append((row[0],))
        # append result of query
        if file[0].endswith('xml'):
            fname = os.path.splitext(os.path.split(file[0])[1])[0]
            res.append(('sysroot/usr/fonts/' + fname + ".mbf", file[0]))
        return res

    def fix_build_order(self):
        pass


def get_db_name():
    return 'agnostic.db'


def get_class(db):
    return agnostic(db)
