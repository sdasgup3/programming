SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD = transpose transpose_parallel_outer_for transpose_parallel_inner_for transpose_parallel_inner_for_ver2 transpose_parallel_collapse_1 optimized_transpose
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

transpose: transpose.o
	$(CC) $(CCFLAGS) -o $@ $<
transpose_parallel_outer_for: transpose_parallel_outer_for.o
	$(CC) $(CCFLAGS) -o $@ $<
transpose_parallel_inner_for: transpose_parallel_inner_for.o
	$(CC) $(CCFLAGS) -o $@ $<
transpose_parallel_inner_for_ver2: transpose_parallel_inner_for_ver2.o
	$(CC) $(CCFLAGS) -o $@ $<
transpose_parallel_collapse_1: transpose_parallel_collapse_1.o
	$(CC) $(CCFLAGS) -o $@ $<
optimized_transpose: optimized_transpose.o
	$(CC) $(CCFLAGS) -o $@ $<

transpose.o: transpose.c
	$(CC) $(CCFLAGS) -c $<
transpose_parallel_outer_for.o: transpose_parallel_outer_for.c
	$(CC) $(CCFLAGS) -c $<
transpose_parallel_inner_for.o: transpose_parallel_inner_for.c
	$(CC) $(CCFLAGS) -c $<
transpose_parallel_inner_for_ver2.o: transpose_parallel_inner_for_ver2.c
	$(CC) $(CCFLAGS) -c $<
transpose_parallel_collapse_1.o: transpose_parallel_collapse_1.c
	$(CC) $(CCFLAGS) -c $<
optimized_transpose.o: optimized_transpose.c
	$(CC) $(CCFLAGS) -c $<
clean:
	rm $(OBJ) $(CMD)

