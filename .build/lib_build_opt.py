import os
import shutil
import module
import util
import subprocess


class module(module.module):
    def prep(self):
        mossy_main = util.get_mossy_path()
        inc = os.path.abspath(os.path.join(mossy_main,
                                           './srcs/' + self.name + '/include'))
        files = [f for f in os.listdir(inc)
                 if os.path.isfile(os.path.join(inc, f))]
        install_path = os.path.abspath(os.path.join(mossy_main,
                                                    './sysroot/usr/include'))
        for file in files:
            shutil.copyfile(os.path.join(inc, file),
                            os.path.join(install_path, file))
        self.set_prepped(True)

    def add_compile_opt(self, compile_opt):
        compile_opt.append('-I./srcs/' + self.name + '/include')

    def get_final(self):
        return 'lib' + self.name + '.a'

    def is_in_module(self, file):
        if os.path.splitext(self.get_final())[0] in os.path.split(file)[1]:
            return True
        return super(module, self).is_in_module(file)

    def clean_file(self, file):
        objects = []
        if os.path.splitext(self.get_final())[0] in os.path.split(file[0])[1]:
            for root,\
                dirs,\
                files in os.walk(os.path.join(util.get_mossy_path(),
                                              file[1] + '/' + self.name)):
                for ofile in files:
                    objects.append(os.path.join(root, ofile))
            old_cd = os.getcwd()
            os.chdir(util.get_mossy_path())
            opts = ['ar', 'r', file[0]]
            opts += objects
            comp = subprocess.Popen(opts)
            os.chdir(old_cd)
            stdout = comp.communicate()[0]
            return comp.returncode is 0
        return False
