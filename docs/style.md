Mossy Recommended Coding Guidelines
===================================
This document aims to describe the recommended coding guidelines for the C and C++ code
on the Mossy operating system. As these guidelines are recommendations, you may choose
to ignore one or more of the recommendations, if you do, point out the violation
and provide rationale for you violation in the source file. However for both rules and
violations rationales _MUST_ be provided so maintainers can recognize when a rule or
violation is either not relevant and can be ignored or a rule that no longer serves a purpose
so the rule can be rewritten/evicted.


File Guidelines
===============
File names and directories should always be written so they they can be typed on a
standard US-104 keyboard without any modifiers with the exception of shift.

##Legal Header

All files should start off with Apache 2.0 license header. The name field should contain
the full name of the appropriate person, the appropriate person depends on the file type, If
the file is a header the name field should be filled in with the maintainer, otherwise the
author should be filled in. The year field should be the four digit creation year with a hyphen
and the two digit year of the last major modification. For example if a file was created in the
year 2013 and last majorly modified in 2015 the year field should be 2013-15. The indentation
can be with either tabs or spaces, however the URL must be indented twice as much as the rest
of text. If tabs are used the indentation should be 1 tab stop. if spaces are used the
indentation may be 1, 2, or 4 spaces.

####rationale
> By ensuring that all file begin with a legal header, we allow anyone who views a single file
> without context to be aware of the license of the file without having to find to find the
> original file project. It also allows an easy place to put who to contact about an inquiries
> about the file.

##Header Files

Header files meant for inclusion by arbitrary files should use an extension of `.h` or `.hpp`
The correct extension to use depends on weather there is code in the header file. If there is
code in the header file use `.hpp` otherwise use `.h`. If you are unsure if your header contains
code the test to use is, if you just include the file in a source file does the resultant `.o`
file's `.text` section grow in size, if it does the header is considered to have code.

####rationale
> Separation of code including and non code including header files allows for the includer of the
> header to recognize that this file increases code size, and compile time for the source file.

###Partial Include Headers
If the header file is not meant for inclusion by arbitrary files and is instead meant for
inclusion by only one, or a small subset of header files the extension should be `.hinc`.
These header should only ever be included by other header. If you need to include a `.hinc`
file outside of libc or libc++ the include should be commented with saying that it is
intentional. ex
~~~{.cpp}
#include <blah.hinc> //style-check: intentional include
~~~
As the comment is parsed by the style checker the comment should be in that format.

####rationale
> Partial Include Headers improve the maintainability of a complex codebase. As most often they
> will be used in the standard library an exemption to the documented inclusion helps keep the
> noise down while still providing security against accidental inclusion in most cases.

After the Legal Header a header file should contain exactly one blank line
then `#pragma once`, then the headers includes and macros,
see Include Ordering for guidelines on ordering includes and macros.
If the header has a namespace there should be another blank line then the namespace deceleration.

####rationale
> Consistent Header beginnings create a more uniform and familiar working environment, and allow
> tools that parse source files to be made simpler by removing edge cases. blank lines provide a
> seperation of concerns from the header, and the code

##Source Files

C source files should always have the extension `.c` and C++ source files should always have
the extension `.cpp`. Source files should usually have an associated header file.
After the Legal Header a source file should contain exactly one blank line
then the headers includes and macros, see Include Ordering for guidelines
on ordering includes and macros. If the header has a namespace there should be another
blank line then the namespace deceleration. C++ source files are required to be in a namespace

####rationale
> Consistent file beginnings create a more uniform and familiar working environment. Namespaced
> Source files reduce the chance of symbol collisions, and create a seperation of concerns across many files. 

##Include Ordering
Includes in files should follow the 'Include What You Use' principle, in that if you require the
definition of function or type, you should include the header that exports that. One exception
to the principle is that a source file that includes an associated header file does not need
to re include the headers included by the associated header.

The ordering of includes should be as follows
  1. associated header file
  2. libc headers (math.h, string.h, ...)
  3. libc++ headers (memory.h, utility.h, ...)
  4. unicode headers (unicode.h, iterators.h, ...)
  5. test headers (test.h, ...)
  6. hal headers (hal/hal.h, hal/paging.h, ...)
  7. sys headers (sys/fb.h, sys/scheduler.h, ...)
  8. arch headers (x86/idt.h, x64/idt.h, ...)
  9. vendor headers (pc/pit.h, ...)
  10. acpi headers (acpi.h, acpi/tables/MADT.h, ...)
  11. other includes ("blah.h", ...)
When you have two or more includes from the same group they should be in alphabetical order.
Macros should, whenever possible, follow the includes, and should not be intermixed.

####rationale
> By removing extraneous includes, compile time can be kept to a minimum, and by not relying on
> headers to have a particular file included, refactors can happen smoother and quicker.  
> Having a consistent location for each header allows you to quickly determine if a header is
> or is not present, much like a dictionary. The order of includes was chosen to reflect the
> "intersting-ness" of the headers, more intersting headers are closer to the bottem and
> faster to scroll up to.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Style Guidelines
================
This portion is for changes that should, after symbol renaming, result in no changes to the
resultant object files. So enforcement of this section is mandatory.

##Indentation and Alignment
Indentation to the current Indent level _MUST_ be done with tabs. Alignment to other lines
_MUST_ be done with spaces. ex
~~~
--->if(...) {
--->--->some_code(a,
--->--->··········b);
--->}
~~~

####rationale
> The combination of Indentation using tabs and Alignment using spaces, often called smart tabs,
> offer a superior editing experience, than either a tab only solution, or a space only solution.
> Both tab only and space only, require that the editor use the indent size that was chosen by the
> file author. However smart tabs allow the editor to use whatever indent size they prefer. ex
~~~
//2 space indent size
->if(...) {
->->some_code(a,
->->··········b);
->}
//4 space indent size
--->if(...) {
--->--->some_code(a,
--->--->··········b);
--->}
//7 space indent size
------>if(...) {
------>------>some_code(a,
------>------>··········b);
------>}
~~~

###Determining Indent Level
Determining Indent Level is a necessary skill when using smart tabs, as you must precisely
determine when the indentation stops and the alignment begins. The Indentation level for a
file starts at 0 at the top and increases by 1 at the start of a block and decreases bu 1
at the end of a block with a few exceptions. Most blocks start with `{` and end with `}`
however not all blocks do. An exception to the Indentation Level is that access modifiers
such as `public:` or `private:` are at 1 less Indentation Level than the blocks normal
Indentation Level ex.
~~~{.cpp}
class foo {
    int bar;
public:
    int foobar;
}
~~~

####rationale
> Having a definite method for determining Indent level is critical to making sure code is consistant
> across revisions and editors. As scope usually changes whenever a new block is created or a block ends
> matching the indent level to the blovk changes, provides a nearly unambigous visual clue to the scope of
> a symbol.

Macros are always at indentaion level 0 irrespectively of the current indent level

####rationale
> As macros always have file scope, and are not effected by the block they are declared in, defining
> macros at indent level 0 provides an accurate visual clue to as to the true scope of the macro.

##Blank Lines
Inside function, blank lines should be avoided. When blank lines are present outside of functions they
should only be used when either required by previous guidelines, or when seperating unrelated blocks of varibles.
There should also be a single blank line between non overloaded functions in header files. ex:
~~~{.cpp}
int sum(int *nums, int count);
double sum(double *nums, int count);

int max(int a, int b);
double max(double a, double b);
~~~

####rationale
> Blank lines offer minimal advantages while increasing the amount of scrollable lines considerably.
> However outside of functions blank lines do have use of grouping related items together.

##Curly Braces
Curly braces are required for all if/for blocks and while blocks that do not fit onto a single line.
An opening curly brace `{` should _ALWAYS_ be the last character in a line. Comments after an opening
curly brace are not permitted. There should be a single space before an opening curly brace. Closing
curly braces `}` should _ALWAYS_ be on their own line with no other content before or after them.
Closing curly braces should be indented to new indent level, that they signify. ex
~~~{.cpp}
	//good
	if(foo) {
		content;
	}
	//bad
	if(foo)
		content;
	if(foo) {content};
	if(foo) content;
	if(foo){
		content;
	}
~~~
####rationale
> Comments after a curly brace, are often better suited to be on the line directly before, instead of
> following the brace. Having a closing curly brace on its own, ensures that the decrease in indent level
> is hard to miss. The forced inclusion of blocks for if and for remove an entire class of hard to spot
> runtime bugs, an exemption for single line while is permitted because the next line can not be confused
> as part of the while loop.

###Trivial Functions
Trivial functions that are both declared and implement at the same time, can be written as a single line.
This is an exception to the requirment that nothing follows an open curly brace, and
that a closing curly brace is on its own line
