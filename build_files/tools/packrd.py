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

def help():
	print "packrd, version 1.0 copyright Matthew Fosdick 2013"
	print "packrd [<output> <input>]"
	print "packs the files in the folder input into the ramdisk output"
	print "if no arguments are passed, 'initrd.rd' is the assumed output"
	print "and 'rdfiles' is the assumed input" 
	
def writeU32(output,num):
	output.write(struct.pack("I",num))

#I like C, going to ignore PEP 8
argv=sys.argv
argc=len(argv)

input=path.abspath("rdfiles")
outputp=path.abspath("initrd.rd")

if argc == 2 and argv[1] == "h":
	help()
	exit(0)

if argc == 3:
	outputp=path.abspath(argv[1])
	input=path.abspath(argv[2])

if not path.exists(input) or not path.isdir(input):
	print "non existent input, or not a directory: "+input
	exit(1)

tmpfiles=os.listdir(input)
files=[]

#gather all the file names
for a in tmpfiles:
	a=path.join(input,a)
	if path.isfile(a):
		files.append(a)
		

output=open(outputp,"wb")

print "writing "+str(len(files))+" files"

#begin with global header
writeU32(output,0xAA55AA55) #magic
writeU32(output,0x1)        #version
writeU32(output,len(files))

#write each individual file header
for f in files:
	#make the name like this "test         txt"(16 char long)
	fname=path.basename(f).split('.')
	name=(fname[0]+"             ")[0:13]
	name+=fname[1][0:3]
	output.write(name)
	writeU32(output,os.path.getsize(f))

#write contents of each files	
for f in files:
	writeU32(output,0x55AA55AA) #magic
	output.write(open(f,"rb").read())

writeU32(output,0xFFFFFFFF)
output.close()
