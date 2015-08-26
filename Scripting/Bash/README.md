for f in `find . -name "*linked.bc"`; do b=$(echo $f | sed 's/\.bc//g'); llvm-dis $b.bc -o $b.ll;  done
