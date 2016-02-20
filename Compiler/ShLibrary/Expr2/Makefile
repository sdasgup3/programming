
.PHONY: clean ars
.PRECIOUS: %.o
all:execute
execute: prog 


prog: a.cpp ars
	echo
#gcc -shared -Wl,-soname,libfoo.so -o ./libfoo.so -L. -lb -ld
	gcc -shared -o libfoo.so -Wl,--whole-archive ./libb.a ./libd.a -Wl,--no-whole-archive
	gcc a.cpp -L. -lfoo  -o $@
	./$@

%.o: %.cpp
	gcc -fPIC -std=c++11 -c   $^ -o $@

lib%.a:%.o
	ar cr $@ $<

ars:	liba.a libb.a libd.a

clean:
	rm -rf lDir *.a *.o *.so prog a.out

