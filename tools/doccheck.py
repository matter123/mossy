#! /usr/bin/python
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
import struct
import os
import os.path as path

doc=os.getcwd()

def check(f):
	f1=combine(doc,f.split('.')[0]+".txt")
	return not path.exists(f1)

def combine(path1, path2):
	return path.normpath(path.join(path1, path2))
	
def help():
	print "doccheck, version 1.0 copyright Matthew Fosdick 2013"
	print "doccheck <resource> [root dir]"
	print "checks the documentation for matching files to each .c .h and .s file"
	print "<resource> is the folder to check in docs in doc/<resource> and files in <resource>"
	print "if [root dir] is specfied treat [root dir] as working directory"




#I like C, going to ignore PEP 8
argv=sys.argv
argc=len(argv)

if(argc!=2 and argc!=3):
	help()
	exit(0)

if(argv[1]=="h"):
	help()
	exit(0)

working=os.getcwd()
if argc==3:
	working=path.abspath(argv[2])

r=combine(working, argv[1])
doc=combine(combine(working,"doc"),argv[1])

hd=0
t=0

for root, dirs, files in os.walk(r):
	for file in files:
		if file.endswith(".h") or file.endswith(".s"):
			t+=1
			f = path.normpath(path.relpath(root,r)+"/"+file)
			if check(f):
				print "warning missing documentation for "+f
			else:
				hd+=1

#for fun
percent=hd/float(t)*100
print str(round(percent,1))+"% of files have documentation"
