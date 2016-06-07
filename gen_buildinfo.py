#format should look like:
#export BUILD_INFO=-DUNIXTIME=0000000 ...
import time
import datetime
import getpass
import subprocess
import salt
import sys

#git info
revision=subprocess.Popen(["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE).communicate()[0]\
    .decode('unicode_escape').strip()
status=subprocess.Popen(
    ["git", "status", "-b","--porcelain"], stdout=subprocess.PIPE).communicate()[0]\
    .decode('unicode_escape')
branch=status.splitlines()[0].split(" ")[1].split("...")[0]
todate=subprocess.call(["git", "diff-index","--cached","--quiet","HEAD","--"]) is 0

compiler_info=subprocess.Popen(sys.argv[1]+" --version", shell=True, stdout=subprocess.PIPE)\
	.communicate()[0].decode("unicode_escape").splitlines()[0].split(" ")
compiler_ver=compiler_info[2]
compiler=compiler_info[0]

name, formula = salt.make_salt(revision)
if not todate:
	name = name + " (dev)"

print("building "+name)

with open('.buildinfo','r') as build:
	if build.readline() == name:
		sys.exit()

with open('src/kernel/stdlib/buildinfo.cpp','w+') as bi:
	bi.write("#pragma GCC diagnostic ignored \"-Wunused-variable\"\n")
	bi.write("const char *BUILD_REV=\""+revision+"\";\n")
	bi.write("const char *BUILD_BRANCH=\""+branch+"\";\n")
	bi.write("const bool  BUILD_DEV="+str(not todate).lower()+";\n")
	bi.write("const char *BUILD_NAME=\""+name+"\";\n")
	bi.write("const char *BUILD_FORMULA=\""+formula+"\";\n")
	bi.write("const char *COMPILER=\""+compiler+"\";\n")
	bi.write("const char *COMPILER_VER=\""+compiler_ver+"\";\n")

with open('.buildinfo','w+') as build:
	build.write(name)
