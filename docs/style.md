Mossy Recommended Coding Guidelines
===================================
This document aims to describe the recommended coding guidelines for the C and C++ code
on the Mossy operating system. As these guidelines are recommendations, you may choose
to ignore one or more of the recommendations, if you do, point out the violation
and provide rationale for you violation in the source file.

Legal Header
------------
All files should start off with Apache 2.0 license header. The name field should contain
the full name of the appropriate person, the appropriate person depends on the file type.
The year field should be the four digit creation year with a hyphen and the two digit year
of the last major modification. For example if a file was created in the year 2013 and last
majorly modified in 2015 the year field should be 2013-15. The indentation can be with either
tabs or spaces, however the URL must be indented twice as much as the rest of text. If tabs
are used the indentation should be 1 tab stop. if spaces are used the indentation may be
1, 2, or 4 spaces.

###rationale
By ensuring that all file begin with a legal header, we allow anyone who views a single file
without context to be aware of the license of the file without having to find to find the
original file project. It also allows an easy place to put who to contact about an inquiries
about the file