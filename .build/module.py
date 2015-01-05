
class module(object):
    def __init__(self, cursor):
        self.cursor = cursor
        self.name = 'None'

    def is_prepped(self):
        self.cursor.execute('SELECT prep FROM modules WHERE name = ?',
                            self.name)
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
            self.cursor.execute('UPDATE modules SET prep = ? WHERE name= ?',
                                (value, self.name))
            if not self.cursor.rowcount:
                self.cursor.execute('INSERT INTO modules (name, prep) VALUES\
                    (?,?)', (self.name, value))

    def add_compile_opt():
        pass

    def prep():
        pass
