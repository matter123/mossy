import util


class module(object):
    def __init__(self, name, cursor):
        self.cursor = cursor
        self.name = name

    def __eq__(self, other):
        return self.name == other.name

    def __ne__(self, other):
        return not self.__eq__(other)

    def is_prepped(self):
        self.cursor.execute('SELECT prep FROM modules WHERE name = ?',
                            (self.name,))
        rows = self.cursor.fetchall()
        if(len(rows)):
            return rows[0][0] == 1
        return False

    def set_prepped(self, value):
        if value:
            value = 1
        else:
            value = 0
        if self.name != 'None':
            self.cursor.execute('UPDATE modules SET prep = ? WHERE name = ?',
                                (value, self.name))
            if not self.cursor.rowcount:
                self.cursor.execute('INSERT INTO modules (name, prep) VALUES\
                    (?,?)', (self.name, value))

    def add_compile_opt(self, compile_opt):
        pass

    def prep(self):
        pass

    def is_in_module(self, file):
        if self.name != 'None':
            file = util.get_db_name(file)
            if 'srcs/' not in file:
                return False
            return self.name in file[5:] and file[5:].index(self.name) == 0\
                and file[5 + len(self.name):][0] == '/'
        return True

    def clean_file(self, file):
        return False

    def get_final(self):
        return 'BAD: ' + self.name
