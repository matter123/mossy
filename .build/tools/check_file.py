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
    "	Copyright 2015 Matthew Fosdick",
    "",
    "	Licensed under the Apache License, Version 2.0 (the \"License\");",
    "	you may not use this file except in compliance with the License.",
    "	You may obtain a copy of the License at",
    "",
    "		http://www.apache.org/licenses/LICENSE-2.0",
    "",
    "	Unless required by applicable law or agreed to in writing, software",
    "	distributed under the License is distributed on an \"AS IS\" BASIS,",
    "	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND," +
    " either express or implied.",  # not a seprate line in source files
    "	See the License for the specific language governing permissions and",
    "	limitations under the License.",
    "*/"]
alt_year = [
    "	Copyright 2013-15 Matthew Fosdick",
    "	Copyright 2014-15 Matthew Fosdick",
    "	Copyright 2014 Matthew Fosdick",
]


def check_file(file):
    pas = True
    header = not (file.endswith(".c") or file.endswith(".cpp"))
    c_line = 0
    headers = []
    with open(file, "r") as f:
        if "srcs/" in file:
            file = file[file.index('srcs/') + 5:]
        for line in f:
            if '\n' not in line:
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
                        print(line)
                        message.error(file + " is missing a #include")
                        pas = False
                        break
                    else:
                        break
        if not pas:
            message.error(file + " failed a mandatory check")
        return pas
