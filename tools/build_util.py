# Copyright 2013 Matthew Fosdick

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

         # http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import fnmatch
import os.path as path
import subprocess


class build_util:
    'utilities for building'

    def __init__(self, build_config):
        self.bc = build_config

    def collectSrcFiles(self):
        #make sure these types are lists
        self.cxxsrcs = []
        self.csrcs = []
        self.ssrcs = []
        self.cxxobjs = []
        self.cobjs = []
        self.sobjs = []
        #used to chop off the absolute path
        abspathlen = len(self.bc.fixpathsrc('.')) + 1

        #loop through all files
        for root, dirnames, filenames in os.walk(self.bc.getSourceFolder()):
            for filename in fnmatch.filter(filenames, '*.c'):
                cfile = path.join(root, filename)
                self.csrcs.append(cfile)
                self.cobjs.append(
                    './' + path.splitext(cfile[abspathlen:])[0] + '.o')

            for filename in fnmatch.filter(filenames, '*.cpp'):
                cxxfile = path.join(root, filename)
                self.cxxsrcs.append(cxxfile)
                self.cxxobjs.append(
                    './' + path.splitext(cxxfile[abspathlen:])[0] + '.o')

            for filename in fnmatch.filter(filenames, '*.s'):
                sfile = path.join(root, filename)
                self.ssrcs.append(sfile)
                self.sobjs.append(
                    './' + path.splitext(sfile[abspathlen:])[0] + '.o')

    def cleanIfRequested(self):
        if self.bc.clean == 1:
            if not self.bc.suffix == 1:
                for obj in (self.cobjs + self.cxxobjs + self.sobjs):
                    try:
                        os.remove(self.bc.fixpathdest(obj))
                    except OSError:
                        pass
            else:
                for i in range(self.bc.getBuildCount()):
                    target = self.bc.getTarget(i + 1)
                    for obj in (
                            self.cobjs + self.cxxobjs + self.sobjs):
                        try:
                            os.remove(
                                self.bc.fixpathdest(obj) + '.' +
                                self.bc.getArchName(target))
                        except OSError:
                            pass

    def needs_rebuild(self, target, obj, src):
        if self.bc.clean == 1:
            return True
        if self.bc.suffix == 1 or self.bc.getBuildCount() == 1:
            try:
                if path.getmtime(
                        self.bc.fixpathdest(
                            self.bc.applySuffix(obj, target))) >\
                        path.getmtime(
                            self.bc.fixpathsrc(src)):
                    return False
            except OSError:
                return True
        return True

    def build(self, target, comp, opt, obj, src):
        ret = "Compiling " + src + ':'
        obj = self.bc.applySuffix(obj, target)
        arg = []
        arg.append(comp)
        arg += opt.split(' ')
        arg.append('-o')
        arg.append(self.bc.fixpathdest(obj))
        arg.append('-c')
        arg.append(self.bc.fixpathsrc(src))
        while '' in arg:
            arg.remove('')

        err = subprocess.Popen(arg, stderr=subprocess.PIPE).communicate()[0]
        if err is not None:
            ret += '\n\t' + err
        else:
            while len(ret) < 71:
                ret += ' '
            ret += 'no errors'
        return ret

    def build_asm(self, target, prep, popt, obj, src):
        ret = "Assembling " + src + ':'
        obj = self.bc.applySuffix(obj, target)
        arg = []
        arg.append(prep)
        arg += popt.split(' ')
        arg += ['-E', '-P', '-o']
        arg.append(self.bc.fixpathdest(obj + '.cpps'))
        arg.append(self.bc.fixpathsrc(src))
        while '' in arg:
            arg.remove('')

        err = subprocess.Popen(arg, stderr=subprocess.PIPE).communicate()[0]
        if err is not None:
            ret += '\n\t' + err
            return ret

        arg = []
        arg.append(self.bc.getProgram('NASM'))
        arg.append(self.bc.getDef('COMPILEOPT', 'NASM', ''))
        arg.append(self.bc.getDef('COMPILEOPT',
                                  self.bc.getArchName(target) + '-NASM', ''))
        arg.append(self.bc.fixpathdest(obj + '.cpps'))
        arg.append('-o')
        arg.append(self.bc.fixpathdest(obj))
        while '' in arg:
            arg.remove('')
        err = subprocess.Popen(arg, stderr=subprocess.PIPE).communicate()[0]
        if err is not None:
            ret += '\n\t' + err
            return ret
        else:
            while len(ret) < 71:
                ret += ' '
            ret += 'no errors'
        try:
            os.remove(self.bc.fixpathdest(obj + '.cpps'))
        except OSError:
            pass
        return ret
