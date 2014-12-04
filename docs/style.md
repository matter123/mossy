Style and coding guidelines
===========================
Style guidelines
----------------

### indentation
Indentation should **ALWAYS** be done with tabs.  
Alignment after indentation to the corect level should **ALWAYS** be done with spaces.  
ex:  
![Indent & Align](http://matter123.github.io/mossy2/indent-align.png "Indent & Align")  
### curly braces
The start curly brace is **ALWAYS** a attached to the first line of a block.  
The end curly brace is **ALWAYS** on its own line and at the same indentation level as the start of the block.  
ex:  
![curly](http://matter123.github.io/mossy2/curly.png "proper Curly brace")  
Braceless while and for loops are acceptable as long as both the loop and the condition fit on a single line.  
Braceless if and else blocks are **NEVER** okay.  
**bad:**  
![bad braceless](http://matter123.github.io/mossy2/no-brace-bad.png "braceless if not allowed")  
**good:**  
![good braceles](http://matter123.github.io/mossy2/no-brace-good.png "no extra whitespace")  
### spaces
Extra spaces around operators and parenthases should be removed whenever possiable.  
**bad:**  
![bad spaces](http://matter123.github.io/mossy2/space-bad.png "extra whitespace")  
**good:**  
![good spaces](http://matter123.github.io/mossy2/space-good.png "no extra whitespace")  
Space to the right of commas when declaring or defining a function is okay.  
There should be a single space between the last ')' and the opening brace of a block.  
### blank lines
Blank lines are acceptable whenever their purpose is to provide seperation of unrelated items  
or in an implementation file sepration of varibles and functions.  
**not ok:**  
![bad blank](http://matter123.github.io/mossy2/blank-bad.png "an unacceptable use of blank lines")  
**ok:**  
![good blank](http://matter123.github.io/mossy2/blank-good.png "an acceptable use of blank lines")  
Coding guidelines
-----------------

###symbol names
All symbols are to be in _underscore\_case_.  
Systems Hungarian is forbidden, Apps Hungarian is acceptable.  
Symbol names in global, namespace, fille, class, struct, union, or function scope should be
descriptive and only use full words and common acronyms and abbreviations.
Symbols named tmp and its derivatives are only acceptable when in block scope.  
One character symbols are resverd and not available for normal use. A list of uses for each
letter is listed below.  
