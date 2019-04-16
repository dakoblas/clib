/*

MIT License

Copyright (c) 2019 Daniel Koblas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* this is a macro that provides a "debug mode" wrapper for printf debug output.  
This can be included and enabled/disabled on a per-source-file basis.

to use, put the following define before the include in the file you want to use this in (in your .c source file):

    #define DEBUG_MODE // comment to disable
    #include "debug.h"

debug mode will be toggled on and off by commenting the "#define DEBUG_MODE" statment.

in your code:

    debug("some debug message...\n");

and you'll get the output with a <debug> tag, the file name, line number, and function, plus your message:

<debug> myfile.c: 27: my_function(): some debug message

the message can use printf-style formatting (since it's really a printf wrapper).

*/

/* NO_DEBUG_MODE exists so we can conditionally turn "undefine" DEBUG_MODE from the command line.  this is kind of a hack, but there is no "undefine" command-line directive for #define's that are in the code. */
#ifndef NO_DEBUG_MODE
/* here we only define DEBUG_MODE if it was already defined in the source code.  
so, if we didn't define NO_DEBUG_MODE, but also didn't define DEBUG_MODE in the source code (i.e. we commented it out), then it won't be defined at all.
this allows us to be able to toggle it on/off both in the source code file AND 
via command line arguments */
#ifdef DEBUG_MODE
#define DEBUG_MODE
#endif
#endif
/*... the end result here is that if we define NO_DEBUG_MODE then it 
will be turned off regardless of any definitions in the source code */


#ifdef DEBUG_MODE
#define debug(...) printf("%s: %d: %s():  ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__)
#else
#define debug(...)
#endif
