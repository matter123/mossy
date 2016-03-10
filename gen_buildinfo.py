#format should look like:
#export BUILD_INFO=-DUNIXTIME=0000000 ...
import time
import datetime
import getpass
import subprocess

try:
    bn = open(".build_number", "r")
    build_number = bn.read().split(" ")
    if int(build_number[0]) == int(datetime.date.today().toordinal()):
        build_number = int(build_number[1]) + 1
        bn.close()
    else:
        build_number = 1
        bn.close()
except:
    build_number = 1
    try:
        bn.close()
    except:
        pass
bn = open(".build_number", "w")
bn.write(str(datetime.date.today().toordinal()) + " " + str(build_number))
bn.close()
build_info = "export BUILD_INFO="
build_info += "-DBUILD_UNIX_TIME=" + str(int(time.time())) + "L "
build_info += "-DBUILD_NUMBER=" + str(int(build_number)) + "L "
build_info += "-DBUILD_DATE=" + str(int(datetime.date.today().toordinal())) + "L "
build_info += "-DBUILD_USERNAME=\\\"" + str(getpass.getuser()) + "\\\" "

out = subprocess.Popen(
    ["git", "status", "-sb"], stdout=subprocess.PIPE).communicate()[0]\
    .decode('unicode_escape')
build_info += "-DBUILD_GIT_BRANCH=\\\"" + out.splitlines()[0].split(" ")[1].split("...")[0] + "\\\" "

bi = open("build_info", "w")
bi.write(build_info)
bi.close
