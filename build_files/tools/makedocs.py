#!/usr/bin/env python
#import sys
import os
import os.path as path
#import time
#import getpass
import subprocess
import shutil


def check_doxygen():
    try:
        subprocess.Popen(['doxygen', '-h'],
                         stderr=subprocess.PIPE, stdout=subprocess.PIPE).wait()
    except:
        return False
    return True


def check_dot():
    try:
        subprocess.Popen(['dot', '-V'],
                         stderr=subprocess.PIPE, stdout=subprocess.PIPE).wait()
    except:
        return False
    return True


if not check_doxygen():
    print('Doxygen not found, Aborting')
    exit(1)

if not check_dot():
    print('Dot(Graphiz) not found, Aborting')
    exit(1)
host = path.dirname(path.realpath(__file__))
if 'tools' in host:
    host = path.dirname(path.dirname(host))
print('deleting old documentation')
shutil.rmtree(path.join(host, 'docs/html'), ignore_errors=True)
os.chdir(host)
print('generating new documentation')
subprocess.Popen(['doxygen', 'docs/Doxyfile']).wait()
