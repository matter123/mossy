#! /usr/bin/python
# Copyright 2014 Matthew Fosdick

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
import re
import subprocess

def help():
	print "symexport, version 1.0 copyright Matthew Fosdick 2014"
	print "symexport <object>"
	print "exports the symbol table of <object> and strips the table"

def writeU32(output,num):
	output.write(struct.pack("I",num))


#I like C, going to ignore PEP 8
symmatch=re.compile("c[0123456789abcdef]{7} [g|l] *[F|0]? *.text"); #remove the c and replace 7 with 8 if you want to catch all symbols
argv=sys.argv
argc=len(argv)
if(argc!=2):
	help()
	exit(0)

if(argv[1]=="h"):
	help()
	exit(0)
out = subprocess.Popen(["objdump","-t",argv[1]], stdout=subprocess.PIPE).communicate()[0]
count=0
strlist=[]
for l in out.splitlines():
	if(symmatch.match(l)):
		strlist.append(l)
		count+=1
if count==0:
	print "no symbols found, aborting"
	exit(1)
export=open(os.path.splitext(argv[1])[0] + ".sym","wb")
writeU32(export,0x5677AB1E); #SYMTABLE
writeU32(export,count);
for l in strlist:
	writeU32(export,0x00E67126) #00ENTRY, yay bad hexspeek
	writeU32(export,int(l[0:7],16))
	demangle=subprocess.Popen(["c++filt","-p",l.split('.text')[1].strip().split(' ')[1]], stdout=subprocess.PIPE).communicate()[0].strip()
	writeU32(export,len(demangle))
	export.write(demangle)
writeU32(export,0xFFFFFFFF)
subprocess.Popen(["strip",argv[1]], stdout=subprocess.PIPE)
print str(count) + " symbols exported and stripped"
export.close()