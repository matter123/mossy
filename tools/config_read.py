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

import os.path as path
try:
    import ConfigParser
except ImportError:
    import configparser as ConfigParser


class build_config:

    "so much boring-ness, please don't read me"

    def __check(self):
        sections = ['BUILD', 'COMPILERS', 'PATHS', 'COMPILEOPT', 'GENERAL']
        for s in sections:
            if not self.config.has_section(s):
                print ('cannot find section [' + s + '], aborting')
                exit(1)

    def __init__(self, destdir, srcdir, config_file='./build.ini'):
        self.destdir = destdir
        self.srcdir = srcdir
        self.config = ConfigParser.RawConfigParser()
        self.config.read(self.fixpathsrc(config_file))
        self.__check()
        self.clean = int(self.getDef('GENERAL', 'CLEAN_ON_BUILD', '0'))
        self.suffix = int(self.getDef('GENERAL', 'SUFFIX_OBJS', '0'))
        self.debug = int(self.getDef('GENERAL', 'DEBUG', '0'))
        self.lib = int(self.getDef('GENERAL', 'LIB', '0'))

    def fixpathsrc(self, rel):
        return path.normpath(path.join(path.realpath(
            self.srcdir), rel))

    def fixpathdest(self, rel):
        return path.normpath(path.join(path.realpath(
            self.destdir), rel))

    def getIndex(self, section, prefix, index):
        if not self.config.has_option(section, 'COUNT'):
            print (
                'cannot find option \'COUNT\' in [' + section + '], aborting')
            exit(1)
        if int(index) > int(self.config.get(section, 'COUNT')):
            print (prefix + str(index) +
                   ' too large to be in section [' + section + '] aborting')
            exit(1)
        if not self.config.has_option(section, prefix + str(index)):
            print (prefix + str(index) +
                   ' not in section [' + section + '] aborting')
            exit(1)
        return self.config.get(section, prefix + str(index))

    def getDef(self, section, option, default):
        if not self.config.has_option(section, option):
            return default
        return self.config.get(section, option)

    def getIndexDef(self, section, prefix, index, default):
        if not self.config.has_option(section, 'COUNT'):
            return default
        if int(index) > int(self.config.get(section, 'COUNT')):
            return default
        if not self.config.has_option(section, prefix + str(index)):
            return default
        return self.config.get(section, prefix + str(index))

    def getBuildCount(self):
        count = self.getDef('BUILD', 'COUNT', None)
        if count is None:
            print ('cannot find option \'COUNT\' in [BUILD], aborting')
            exit(1)
        return int(count)

    def getTarget(self, target):
        return self.getIndex('BUILD', 'BUILD', target).split('-')

    def getCompilerPrefix(self, target):
        return self.getIndex('COMPILERS', 'PREFIX', int(target[0]))

    def getArchName(self, target):
        return self.getIndex('COMPILERS', 'ARCH', int(target[0]))

    def getVendor(self, target):
        return self.getIndex('VENDOR' + self.getArchName(target),
                             'VENDOR', int(target[1]))

    def getSourceFolder(self):
        return self.fixpathsrc(self.getDef('GENERAL', 'SOURCEFOLDER', 'src'))

    def collateOpts(self, target, lang):
        args = self.getDef('COMPILEOPT', 'GLOBAL', '') + ' '
        inc = self.getDef('COMPILEOPT', 'GLOBALINC', '').split(' ')
        for i in range(len(inc)):
            args += '-I' + self.fixpathsrc(inc[i][2:]) + ' '
        args += self.getDef('COMPILEOPT', 'GLOBAL-' + lang, '') + ' '
        args += self.getDef('COMPILEOPT', 'C' + target[0], '') + ' '
        args += self.getDef(
            'COMPILEOPT', 'C' + target[0] + '-' + lang, '') + ' '

        friend_arch = self.getArchName(target)

        args += self.getDef(
            'COMPILEOPT', friend_arch + 'V' + target[1], '') + ' '
        args += self.getDef(
            'COMPILEOPT', friend_arch + 'V' + target[1] + '-' + lang, '') + ' '
        args += '-DARCH=' + self.getIndex(
            'COMPILERS', 'DEF', int(target[0])) + ' '
        args += '-DVENDOR=' + self.getIndex(
            'VENDOR' + friend_arch, 'DEF', int(target[1])) + ' '
        if self.debug == 1:
            args += '-DDEBUG '
        args = ' '.join(args.split(' '))
        return args

    def getPathComp(self, target):
        return self.getIndex('PATHS', 'PATHC', target[0])
