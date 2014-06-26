#! /usr/bin/env python3
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

sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)

main = config_read.build_config(
    path.realpath(os.getcwd()), path.dirname(path.realpath(__file__)))

main_util = build_util.build_util(main)

buildc = main.getBuildCount()

print 'building ' + str(buildc) + ' targets'

targets = []
for i in range(buildc):
    target = main.getTarget(i + 1)
    targets.append(target)
    comp = main.getCompilerPrefix(target)
    vendor = main.getVendor(target)
    print '(' + comp + ',' + vendor + ')'

# start actual build
gen_build_info.gen(main.srcdir)

main_util.collectSrcFiles()

main_util.cleanIfRequested()

for target in targets:

    cxxopt = main.collateOpts(target, 'CXX')

    copt = main.collateOpts(target, 'C')

    sopt = main.collateOpts(target, 'S')

    # CXX = (getIndex('PATHS', 'PATHC', int(target[0])) + '/' +
    #        getIndex('COMPILERS', 'PREFIX', int(target[0])) + '-gcc ' + cxxopt).split(' ')
    # for i in range(len(cxxobjs)):
    #     fin4 = [
    #         '-o', append(suffix_objs, fixpathcwd(
    #             cxxobjs[i]), '.' + friend_arch), '-c',
    #         fixpath(cxxfiles[i])]
    #     # print CXX+fin4
    #     try:
    #         os.makedirs(path.dirname(fixpathcwd(cxxobjs[i])))
    #     except OSError:
    #         pass
    #     err = subprocess.Popen(CXX + fin4, stdout=subprocess.PIPE, stderr=subprocess.PIPE)\
    #         .communicate()[1]
    #     if err:
    #         print err
