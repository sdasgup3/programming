
all:prog


execute: prog 
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./prog

prog: prog.c lib
	echo
	gcc prog.c -lfoo -L ./lDir -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./prog
	echo
	gcc prog.c mod3.o -lfoo -L ./lDir -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./prog
	echo
	gcc prog.c -lfoo -L ./lDir mod3.o -o $@
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./prog
	#Or u may use
	#gcc prog.c -lbar -L ./lDir -o $@

%.o: %.c
	gcc -c -fPIC  $^ -o $@


lib: mod1.o mod2.o mod3.o
	rm -rf lDir && mkdir lDir
	echo
	gcc -shared -Wl,-soname,libbar.so -o lDir/libfoo.so mod1.o mod2.o
	# Cannot do gcc -shared -Wl,-soname,libbar.so -o lDir/libfoo.so mod1.o mod2.o mod3.o
	# Otherwise will get multiple definition of `x1' error 
	cd lDir && ln -s libfoo.so libbar.so && cd ..

clean:
	rm -rf lDir *.o *.so prog a.out

.PHONY: clean lib


