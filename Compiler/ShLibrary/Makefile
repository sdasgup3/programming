
all:execute


execute: prog1 prog2 

prog1: prog.c shlib
	echo
	g++ -std=c++11 prog.c ./lDir/libfoo.so mod2.o  -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH ./$@
	#Setting LD_LIBRARY_PATH is necessary
	echo
	gcc prog.c -lfoo  -L ./lDir mod2.o  -o $@
	# If u use gcc prog.c -lfoo  -L ./lDir -o $@
	# undefined reference to `x2` 
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@
	echo
	gcc prog.c mod3.o mod2.o -lfoo -L ./lDir -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@
	echo
	gcc prog.c -lfoo -L ./lDir mod2.o mod3.o -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@

	#Or u may use
	#gcc prog.c -lbar -L ./lDir -o $@

prog2:	prog.c ars
	echo
	gcc prog.c -L. -lfoo -L ./lDir -lmod2  -o $@
	# Using gcc prog.c -L. -lfoo -L ./lDir -lmod2 -lmod2   -o $@
	# is OK!
	# Using gcc prog.c -L. -lmod2 -lfoo -L ./lDir   -o $@
	# is NOT OK!
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@
	echo
	gcc prog.c -L. -L./lDir  ./lDir/libfoo.so -lmod2 -lmod3  -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@
	echo
	gcc prog.c -L. -L./lDir -lmod3  ./lDir/libfoo.so -lmod2   -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./$@
	


%.o: %.c
	g++ -std=c++11  -c -fPIC  $^ -o $@
lib%.a:%.o
	ar cr $@ $<

ars:	libmod1.a libmod2.a libmod3.a

shlib: mod1.o mod2.o mod3.o
	rm -rf lDir && mkdir lDir
	echo
	g++ -std=c++11 -shared -Wl,-soname,libbar.so -o lDir/libfoo.so mod1.o 
	# Cannot do gcc -shared -Wl,-soname,libbar.so -o lDir/libfoo.so mod1.o mod2.o mod3.o
	# Otherwise will get multiple definition of `x1' error 
	cd lDir && ln -s libfoo.so libbar.so && cd ..

clean:
	rm -rf lDir *.o *.so prog a.out *.a

.PHONY: clean lib


