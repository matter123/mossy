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
import sys
import fnmatch
import os.path as path
import subprocess
import ConfigParser

def append(do,a,b):
	if do==1:
		return a+b
	return a

def fixpath(rel):
	return path.normpath(path.join(path.dirname(path.realpath(__file__)),rel))

def fixpathcwd(rel):
	return path.normpath(path.join(path.realpath(os.getcwd()),rel))

def getIndex(section,prefix,index):
	if not config.has_option(section,'COUNT'):
		print 'cannot find option \'COUNT\' in ['+section+'], aborting'
		exit(1)
	if index > int(config.get(section,'COUNT')):
		print prefix+str(index)+' too large to be in section ['+section+'] aborting'
		exit(1)
	if not config.has_option(section,prefix+str(index)):
		print prefix+str(index)+' not in section ['+section+'] aborting'
		exit(1)
	return config.get(section,prefix+str(index))

def getDef(section,option,default):
	if not config.has_option(section,option):
		return default
	return config.get(section,option)

def getIndexDef(section,prefix,option,default):
	if not config.has_option(section,'COUNT'):
		return default
	if index > int(config.get(section,'COUNT')):
		return default
	if not config.has_option(section,prefix+str(index)):
		return default
	return config.get(section,prefix+str(index))

sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
config=ConfigParser.RawConfigParser()
config.read(fixpath('./build.ini'))
sections=['BUILD','COMPILERS','PATHS','COMPILEOPT']
for s in sections:
	if not config.has_section(s):
		print 'cannot find section ['+s+'], aborting'
		exit(1)
buildc=0
if config.has_option('BUILD','COUNT'):
	buildc=config.getint('BUILD','COUNT')
else:
	print 'cannot find option \'COUNT\' in [BUILD], aborting'
	exit(1)
print 'building '+str(buildc)+' targets'
targets=[]
for i in range(buildc):
	target=getIndex('BUILD','BUILD',i+1).split('-')
	targets.append(target)
	comp=getIndex('COMPILERS','PREFIX',int(target[0]))
	vendor=getIndex('VENDOR'+getIndex('COMPILERS','ARCH',int(target[0])),'VENDOR',int(target[1]))
	print '(' + comp + ',' + vendor + ')'

#start actual build
subprocess.call([fixpath('./tools/gen_build_info.py')])
cxxfiles=[]
cfiles=[]
sfiles=[]
cxxobjs=[]
cobjs=[]
sobjs=[]
for root, dirnames, filenames in os.walk(fixpath('src')):
	for filename in fnmatch.filter(filenames, '*.c'):
		cfiles.append(path.join(root, filename))
	for filename in fnmatch.filter(filenames, '*.cpp'):
		cxxfiles.append(path.join(root, filename))
	for filename in fnmatch.filter(filenames, '*.s'):
		sfiles.append(path.join(root, filename))
#print cxxfiles
#print cfiles
#print sfiles
abspathlen=len(fixpath('.'))+1
for cxxfile in cxxfiles:
	cxxobjs.append('./'+path.splitext(cxxfile[abspathlen:])[0]+'.o')
for cfile in cfiles:
	cobjs.append('./'+path.splitext(cfile[abspathlen:])[0]+'.o')
for sfile in sfiles:
	sobjs.append('./'+path.splitext(sfile[abspathlen:])[0]+'.o')

suffix_objs=int(getDef('GENERAL','SUFFIX_OBJS',0))
clean=int(getDef('GENERAL','CLEAN_ON_BUILD',1))
debug=int(getDef('GENERAL','DEBUG',0))

if clean==1:
	if suffix_objs==0:
 		for obj in (cobjs+cxxobjs+sobjs):
 			try:
 				os.remove(fixpathcwd(obj))
 			except OSError:
 				pass
 	else:
 		for target in targets:
 			friend_arch=getIndex('COMPILERS','ARCH',int(target[0]))
 			for obj in (cobjs+cxxobjs+sobjs):
 				try:
 					os.remove(fixpathcwd(obj+'.'+friend_arch))
 				except OSError:
 					pass


for target in targets:
	#GLOBAL GLOBALINC GLOBAL-CXX C+target[0] C+target[0]-CXX
	cxxopt=getDef('COMPILEOPT','GLOBAL','')+' '
	inc=getDef('COMPILEOPT','GLOBALINC','').split(' ')
	for i in range(len(inc)):
		cxxopt+='-I'+fixpath(inc[i][2:])+' '
	cxxopt+=getDef('COMPILEOPT','GLOBAL-CXX','')+' '+getDef('COMPILEOPT','C'+target[0],'')+' '
	cxxopt+=getDef('COMPILEOPT','C'+target[0]+'-CXX','')+' '
	friend_arch=getIndex('COMPILERS','ARCH',int(target[0]))
	cxxopt+=getDef('COMPILEOPT',friend_arch+'V'+target[1],'')+' '
	cxxopt+=getDef('COMPILEOPT',friend_arch+'V'+target[1]+'-CXX','')+' '
	cxxopt+='-DARCH='+getIndex('COMPILERS','DEF',int(target[0]))+' '
	cxxopt+='-DVENDOR='+\
		getIndex('VENDOR'+getIndex('COMPILERS','ARCH',int(target[0])),'DEF',int(target[1]))+' '
	if debug:
		cxxopt+='-DDEBUG '
	cxxopt=' '.join(cxxopt.split())

	copt=getDef('COMPILEOPT','GLOBAL','')+' '+getDef('COMPILEOPT','GLOBALINC','')+' '
	copt+=getDef('COMPILEOPT','GLOBAL-C','')+' '+getDef('COMPILEOPT','C'+target[0],'')+' '
	copt+=getDef('COMPILEOPT','C'+target[0]+'-C','')+' '
	copt+=getDef('COMPILEOPT',friend_arch+'V'+target[1],'')+' '
	copt+=getDef('COMPILEOPT',friend_arch+'V'+target[1]+'-C','')
	copt+='-DARCH='+getIndex('COMPILERS','DEF',int(target[0]))+' '
	copt+='-DVENDOR='+\
		getIndex('VENDOR'+getIndex('COMPILERS','ARCH',int(target[0])),'DEF',int(target[1]))+' '
	if debug:
		copt+='-DDEBUG '
	copt=' '.join(copt.split())

	sopt=getDef('COMPILEOPT','GLOBAL','')+' '+getDef('COMPILEOPT','GLOBALINC','')+' '
	sopt+=getDef('COMPILEOPT','GLOBAL-S','')+' '+getDef('COMPILEOPT','C'+target[0],'')+' '
	sopt+=getDef('COMPILEOPT','C'+target[0]+'-S','')+' '
	sopt+=getDef('COMPILEOPT',friend_arch+'V'+target[1],'')+' '
	sopt+=getDef('COMPILEOPT',friend_arch+'V'+target[1]+'-S','')
	sopt+='-DARCH='+getIndex('COMPILERS','DEF',int(target[0]))+' '
	sopt+='-DVENDOR='+\
		getIndex('VENDOR'+getIndex('COMPILERS','ARCH',int(target[0])),'DEF',int(target[1]))+' '
	if debug:
		sopt+='-DDEBUG '
	sopt=' '.join(sopt.split())
	print len(cxxobjs)
	CXX=(getIndex('PATHS','PATHC',int(target[0]))+'/'+ \
		getIndex('COMPILERS','PREFIX',int(target[0]))+'-gcc '+cxxopt).split(' ')
	for i in range(len(cxxobjs)):
		fin4=['-o',append(suffix_objs,fixpathcwd(cxxobjs[i]),'.'+friend_arch),'-c',
		fixpath(cxxfiles[i])]
		#print CXX+fin4
		try:
			os.makedirs(path.dirname(fixpathcwd(cxxobjs[i])))
		except OSError:
			pass
		err=subprocess.Popen(CXX+fin4, stdout=subprocess.PIPE,stderr=subprocess.PIPE)\
			.communicate()[1]
		if err:
			print err