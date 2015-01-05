import message
import fnmatch
import os
import check_file

dirty = []


def get_mtime(file):
    return os.stat(os.path.abspath(os.path.join('../', file)))[9]


def build(conn, cursor):
    for root, dirs, files in os.walk(os.path.join(os.getcwd(), "../srcs")):
        filter = fnmatch.filter(files, "*.cpp")
        filter += fnmatch.filter(files, "*.c")
        filter += fnmatch.filter(files, "*.h")
        for file in filter:
            file = os.path.abspath(os.path.join(root, file))
            file = "./" + file[file.index('srcs/'):]
            displayfile = file[7:]
            cursor.execute('SELECT mod_date, id FROM files WHERE name = ?',
                           (displayfile,))
            rows = cursor.fetchall()
            if len(rows):
                if rows[0][0] == get_mtime(file):
                    message.info(displayfile + " is clean")
                else:
                    dirty.append(file)
                    message.info(displayfile + " is dirty")
                    cursor.execute('DELETE FROM depends WHERE file = ?',
                                   (rows[0][1],))
            else:
                dirty.append(file)
                cursor.execute('''INSERT INTO files
                                  (name, mod_date) VALUES (?,?)''',
                               (displayfile, 0))
                message.info(displayfile + " is new")
    conn.commit()
    for file in dirty:
        depends = check_file.check_file(os.path.abspath(os.path.join('../',
                                                                     file)))
        if len(depends) and depends[0] == "-1":
            break
        print(depends)
