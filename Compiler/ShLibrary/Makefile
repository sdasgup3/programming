
all:execute


execute: prog 
	LD_LIBRARY_PATH=./lDir:$$LD_LIBRARY_PATH	./prog

prog: prog.c lib
	gcc prog.c -lfoo -L ./lDir -o $@
	#Or u may use
	#gcc prog.c -lbar -L ./lDir -o $@

%.o: %.c
	gcc -c -fPIC  $^ -o $@


lib: mod1.o mod2.o mod3.o
	rm -rf lDir && mkdir lDir
	gcc -shared -Wl,-soname,libbar.so -o lDir/libfoo.so $^ 
	cd lDir && ln -s libfoo.so libbar.so && cd ..

clean:
	rm -rf lDir *.o *.so prog a.out

.PHONY: clean lib


