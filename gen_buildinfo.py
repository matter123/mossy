import hashlib
import subprocess
import salt
import sys
import os


#git info
revision=subprocess.Popen(["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE).communicate()[0]\
    .decode('unicode_escape').strip()
status=subprocess.Popen(
    ["git", "status", "-b","--porcelain"], stdout=subprocess.PIPE).communicate()[0]\
    .decode('unicode_escape')
branch=status.splitlines()[0].split(" ")[1].split("...")[0]

compiler_info=subprocess.Popen(sys.argv[1]+" --version", shell=True, stdout=subprocess.PIPE)\
	.communicate()[0].decode("unicode_escape").splitlines()[0].split(" ")
compiler_ver=compiler_info[2]
compiler=compiler_info[0]

diff=subprocess.Popen(["git", "diff", "HEAD","--"], stdout=subprocess.PIPE).communicate()[0]
todate = diff == b''

if todate:
	diff=subprocess.Popen(["git", "diff", "HEAD^..HEAD"], stdout=subprocess.PIPE).communicate()[0]


name, formula = salt.make_salt(hashlib.sha1(diff).hexdigest())
if not todate:
	name = name + " (dev)"

print("building "+name)
if os.path.exists('.buildinfo'):
	with open('.buildinfo','r+') as build:
		if build.readline() == name:
			sys.exit()

with open('src/kernel/stdlib/buildinfo.cpp','w+') as bi:
	bi.write("#pragma GCC push \"-Wunused-variable\"\n")
	bi.write("#pragma GCC diagnostic ignored \"-Wunused-variable\"\n")
	bi.write("const char *BUILD_REV=\""+revision+"\";\n")
	bi.write("const char *BUILD_BRANCH=\""+branch+"\";\n")
	bi.write("const bool  BUILD_DEV="+str(not todate).lower()+";\n")
	bi.write("const char *BUILD_NAME=\""+name+"\";\n")
	bi.write("const char *BUILD_FORMULA=\""+formula+"\";\n")
	bi.write("const char *COMPILER=\""+compiler+"\";\n")
	bi.write("const char *COMPILER_VER=\""+compiler_ver+"\";\n")
	bi.write("#pragma GCC pop \"-Wunused-variable\"\n")

with open('.buildinfo','w+') as build:
	build.write(name)
