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
            line = line.rstrip('\n')
            pas = True
            if c_line <= 14:
                if line != cur_head[c_line]:
                    if c_line == 1:
                        try:
                            for a_line in alt_year:
                                if a_line == line:
                                    c_line += 1
                                    print("[WARNING] " + file +
                                          " has an outdated legal")
                                    raise Exception
                        except:
                            continue
                    print("[ERROR]   " + file +
                          " failed legal check on line " +
                          str(c_line + 1))
                    print("[INFO]    got \"" + line + "\" expected \"" +
                          cur_head[c_line] + "\"")
                    pas = False
                    break
                c_line += 1
            elif header is True:
                if c_line == 15:
                    if line != "":
                        print("[ERROR]   " + file +
                              " is missing blank line after legal")
                        pas = False
                        break
                elif c_line == 16:
                    if line != "#pragma once":
                        print("[ERROR]   " + file +
                              " is missing #pragma once")
                        pas = False
                        break
                c_line += 1
            else:
                if line != "" and len(headers) == 0:
                    if not line.startswith("#include"):
                        print("[ERROR]   " + file +
                              " is missing a #include")
                        pas = False
                        break
            if not line.startswith("#include"):
                continue
            line = line[len("#include"):]
            if not line.startswith(" "):
                print("[WARNING] " + line +
                      " is missing a space after #include")
            else:
                line = line[1:]
            if line[0] == " ":
                print("[WARNING] " + line +
                      " has to many spaces after #include")
            elif line[0] == "<":
                headers.append(line[0:line[0:].index(">") + 2])
            elif line[0] == "\"":
                headers.append(line[0:line[1:].index("\"") + 2])
            else:
                print("[ERROR]   " + line +
                      " unknown #include")

        if not pas:
            print("[ERROR]   " + file +
                  " failed a mandatory check")
        print(headers)
import sys
if len(sys.argv) > 1:
    check_file(sys.argv[1])
