Tips
=====

```
To compile all the files like
a/b/c.bc
z.bc
to
a/b/c.ll
z.ll
for f in `find . -name "*linked.bc"`; do b=$(echo $f | sed 's/\.bc//g'); llvm-dis $b.bc -o $b.ll;  done
```
