Call-Strings. 
The call-graph of a program is a labeled graph in which each node represents
a procedure, each edge represents a call, and the label on an edge represents
the call-site corresponding to the call represented by the edge. 

A call-string is a sequence of call-sites (c1c2 . . . cn) such that call-site c1 belongs to the entry procedure,
and there exists a path in the call-graph consisting of edges with labels c1, c2, . . . , cn.
CallString is the set of all call-strings in the program.
A call-string suffix of length k is either (c1c2 . . . ck) or (∗c1c2 . . . ck), where c1,
c2, . . . , ck are call-sites. (c1c2 . . . ck) represents the string of call-sites c1c2 . . . ck.
(∗c1c2 . . . ck), which is referred to as a saturated call-string, represents the set {cs|cs ∈
CallString, cs = πc1c2 . . . ck, and |π| ≥ 1}. CallStringk is the set of saturated callstrings
of length k, plus non-saturated call-strings of length ≤ k.

Referenecs:
M. Sharir and A. Pnueli. Two approaches to interprocedural data flow analysis. In Program
Flow Analysis: Theory and Applications, chapter 7, pages 189–234. Prentice-Hall, 1981.


Value Set:
T. Reps, G. Balakrishnan, and J. Lim. Intermediate representation recovery from low-level
code. In PEPM, 2006.
G. Balakrishnan and T. Reps. Analyzing memory accesses in x86 executables. In Comp.
Construct., 2004.

Why dissambly
--------------
Recovering functional IR is also valuable for legacy code for
which the source code has been lost. With a tool to recover IR,
users of legacy code can fix bugs in such codes, modify the IR functionality
or even port the code to new hardware systems. They may
also optimize old code by doing more aggressive optimizations or
making portions of the code parallel.
