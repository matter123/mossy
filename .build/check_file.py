#!/usr/bin/env python
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
import message

cur_head = [
    "/*",
    "    Copyright 2014 Matthew Fosdick",
    "",
    "    Licensed under the Apache License, Version 2.0 (the \"License\");",
    "    you may not use this file except in compliance with the License.",
    "    You may obtain a copy of the License at",
    "",
    "     http://www.apache.org/licenses/LICENSE-2.0",
    "",
    "    Unless required by applicable law or agreed to in writing, software",
    "    distributed under the License is distributed on an \"AS IS\" BASIS,",
    "    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
    "    See the License for the specific language governing permissions and",
    "    limitations under the License.",
    "*/"]
alt_year = [
    "    Copyright 2013 Matthew Fosdick",
    "    Copyright 2013-14 Matthew Fosdick",
    "    Copyright 2013-2014 Matthew Fosdick",
]


def check_file(file):
    header = not (file.endswith(".c") or file.endswith(".cpp"))
    c_line = 0
    headers = []
    with open(file, "r") as f:
        if "srcs/" in file:
            file = file[file.index('srcs/') + 5:]
        for line in f:
            if not '\n' in line:
                message.error(file + " is missing blank line at the end")
                pas = False
                break
            line = line.rstrip('\n')
            pas = True
            if c_line <= 14:
                if line != cur_head[c_line]:
                    if c_line == 1:
                        try:
                            for a_line in alt_year:
                                if a_line == line:
                                    c_line += 1
                                    message.warning(file +
                                                    " has an outdated legal")
                                    raise Exception
                        except:
                            continue
                    message.error(file + " failed legal check on line " +
                                  str(c_line + 1))
                    message.info("got \"" + line + "\" expected \"" +
                                 cur_head[c_line] + "\"")
                    pas = False
                    break
                c_line += 1
            elif header is True:
                if c_line == 15:
                    if line != "":
                        message.error(file +
                                      " is missing blank line after legal")
                        pas = False
                        break
                elif c_line == 16:
                    if line != "#pragma once":
                        message.error(file + " is missing #pragma once")
                        pas = False
                        break
                c_line += 1
            else:
                if line != "" and len(headers) == 0:
                    if not line.startswith("#include"):
                        message.error(file + " is missing a #include")
                        pas = False
                        break
            if not line.startswith("#include"):
                continue
            line = line[len("#include"):]
            if not line.startswith(" "):
                message.warning(line +
                                " is missing a space after #include")
            else:
                line = line[1:]
            if line[0] == " ":
                message.warning(line +
                                " has to many spaces after #include")
            elif line[0] == "<":
                headers.append(line[0:line[0:].index(">") + 1])
            elif line[0] == "\"":
                headers.append(line[0:line[1:].index("\"") + 1])
            else:
                message.error(line + " unknown #include")

        if not pas:
            message.error(file + " failed a mandatory check")
            headers = ['-1']  # sentenial for a bad file
        return headers
import os
import os.path as path
import fnmatch
for root, dirs, files in os.walk(os.getcwd()):
    for file in fnmatch.filter(files, "*.cpp"):
        path.join(root, file) + " " + str(check_file(path.join(root, file)))
    for file in fnmatch.filter(files, "*.c"):
        path.join(root, file) + " " + str(check_file(path.join(root, file)))
    for file in fnmatch.filter(files, "*.h"):
        path.join(root, file) + " " + str(check_file(path.join(root, file)))
