#! /usr/bin/env python2
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
