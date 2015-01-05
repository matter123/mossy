import os
import fnmatch
import util
import xml.etree.ElementTree as ET


def get_db_name():
    return 'agnostic'


def update_depends(mossy_fol, cur, file):
    dbfile = util.get_db_name(file)
    if file.endswith('xml'):
        tree = ET.parse(file)
        fol = os.path.split(os.path.abspath(file))[0]
        png = util.get_db_name(os.path.join(fol, tree.getroot().attrib["png"]))
        cur.execute('DELETE FROM depends WHERE file = ? ', (dbfile,))
        cur.execute('INSERT INTO depends VALUES (?,?)', (dbfile, png))


def check_mtime(cur, file):
    dbfile = util.get_db_name(file)
    #check mtime in db using dbfile compare to os.stat
    #if not exist in db or stat mtime is newer return True
    cur.execute('SELECT mtime FROM files WHERE file = ?',
                (dbfile,))
    res = cur.fetchall()
    if len(res):
        return res[0][0] < os.stat(file).st_mtime
    return True


def get_dirty_files(mossy_fol, db):
    cur = db.cursor()
    dirty = []
    for root, dirs, files in os.walk(os.path.join(mossy_fol, 'srcs/fonts')):
        for file in fnmatch.filter(files, '*.xml'):
            if check_mtime(cur, file):
                dirty.append(file)
                update_depends(file, cur)
        for file in fnmatch.filter(files, '*.png'):
            if check_mtime(cur, file):
                dirty.append(file)


def get_depends(mossy_fol, db, file):
    cur = db.cursor()
    dbfile = util.get_db_name(file)
    cur.execute('SELECT file FROM depends WHERE depend = ?',
                (dbfile, ))
    res = []
    for row in cur:
        res.append(row[0])
    #append result of query
    if file.endswith('xml'):
        fname = os.path.splitext(os.path.split(file)[1])[0]
        res.append('sysroot/usr/fonts/' + fname + ".mbf")


def clean_file(mossy_fol, db, file):
    pass
