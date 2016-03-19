Arithmetic
-----------
```
Comments a delimited by double quotes; strings are delimited by single quotes. Arithmetic is infix, but there is no operator precedence: you must use parentheses in compound expressions. Self overloads several operators; the lack of precedence prevents confusion. Try typing these into the interpreter:
(2 * 3) + 2                   "prints 8"
(2 @ 3) + (1 @ -2)            "addition of two points; prints 3 @ 1" 
'hello' , ' ' , 'there'       "comma concatenates strings"
(2 & 'foo' & 4.5) asSequence  "a sequence of integer, string, float"
(2 & 'foo' & 4.5) asList      "a list"
(2 & 'foo' & 4.5) asSet       "a set"
(('a' @ 2) & ('b' @ 3)) asDictionary  "a dictionary"
To create the various kinds of collections, we use '&' to make a generic collection and then "type-cast" it.
```
Adding variables to object shell
```
_AddSlots: (|prototypes*=()|).
prototypes _AddSlots: (|ob3= (|s1=0. s2<-0. s3. s4<-nil|)|)
ob3 s2: 7.
ob3 s2 // Or prototypes ob3 s2
_AddSlots: (|x|)
x: 3
ob3 x:10
ob3 x:x+1   // 4, as x is the one in the shell object
ob3 x:10
ob3 x:ob3 x  + 1  //11
ob3 s4:100
ob3 _AddSlots: (|f=(s4: s4 + 100)|)
ob3 s4
_AddSlots: (|y|)
_AddSlots: (|g=(y: ob3 f)|)
```
For more 
http://alumni.media.mit.edu/~tpminka/PLE/self/self-tut.html


The feature that distinguishes a method object from a data object is that it has code, whereas a data object does not. Evaluating a method object does not simply return the object itself, as with simple data objects; rather, its code is executed and the resulting value is returned.

```
5 min: 4 Max: 7
is the single message min:Max: sent to 5 with arguments 4 and 7, whereas

5 min: 4 max: 7
involves two messages: first the message max:sent to 4 and taking 7 as its argument, and then the message min: sent to 5, taking the result of (4 max: 7) as its argument.

5 min: 6 min: 7 Max: 8 Max: 9 min: 10 Max: 11
is interpreted as
5 min: (6 min: 7 Max: 8 Max: (9 min: 10 Max: 11))
The association order and capitalization requirements are intended to reduce the number of parentheses necessary in Self code. For example, taking the minimum of two slots mand nand storing the result into a data slot i may be written as
i: m min: n
```
