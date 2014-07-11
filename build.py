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

import sys
import os
import os.path as path
sys.path.append(
    path.normpath(path.join(path.dirname(path.realpath(__file__)), "./tools")))
import config_read
import build_util
import gen_build_info


def printu(string):
    print(string)
    sys.stdout.flush()

main = config_read.build_config(
    path.realpath(os.getcwd()), path.dirname(path.realpath(__file__)))

main_util = build_util.build_util(main)

buildc = main.getBuildCount()

printu('building ' + main.getName() + ' (' + str(buildc) + ')targets')

targets = []
for i in range(buildc):
    target = main.getTarget(i + 1)
    targets.append(target)

# start actual build
gen_build_info.gen(main.srcdir)

main_util.collectSrcFiles()

main_util.cleanIfRequested()

for target in targets:
    printu('\nTarget=' + main.getFriendlyName(target))
    cxxopt = main.collateOpts(target, 'CXX')
    copt = main.collateOpts(target, 'C')
    sopt = main.collateOpts(target, 'S')
    CC = main.getPathComp(target) + \
        '/' + main.getCompilerPrefix(target) + '-gcc'
    CXX = main.getPathComp(target) + \
        '/' + main.getCompilerPrefix(target) + '-g++'
    CPP = main.getPathComp(target) + \
        '/' + main.getCompilerPrefix(target) + '-cpp'

    for i in range(len(main_util.cobjs)):
        cobj = main_util.cobjs[i]
        csrc = main_util.csrcs[i]
        if main_util.needs_rebuild(target, cobj, csrc):
            main_util.build(target, CC, copt, cobj, csrc)

    for i in range(len(main_util.cxxobjs)):
        cxxobj = main_util.cxxobjs[i]
        cxxsrc = main_util.cxxsrcs[i]
        if main_util.needs_rebuild(target, cxxobj, cxxsrc):
            printu(main_util.build(target, CXX, cxxopt, cxxobj, cxxsrc))

    for i in range(len(main_util.sobjs)):
        sobj = main_util.sobjs[i]
        ssrc = main_util.ssrcs[i]
        if main_util.needs_rebuild(target, sobj, ssrc):
            printu(main_util.build_asm(target, CPP, sopt, sobj, ssrc))

    #build libs
    libs = []
    for i in range(main.getLibCount()):
        libs.append(i + 1)

    for lib in libs:
        printu("building " + main.getLibName(lib))
