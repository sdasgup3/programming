SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD = tiled_transpose tiled_transpose_exp_with_block_size tiled_transpose_with_loop_unroll tiled_transpose_with_loop_unroll_loop_count_exp tiled_transpose_with_loop_unroll_parallel_for_ii tiled_transpose_with_loop_unroll_parallel_for_jj tiled_transpose_with_loop_unroll_parallel_for_i tiled_transpose_with_loop_unroll_parallel_for_j tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_1 tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_2 optimized_tiled_transpose

SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

tiled_transpose: tiled_transpose.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose.o: tiled_transpose.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_exp_with_block_size: tiled_transpose_exp_with_block_size.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_exp_with_block_size.o: tiled_transpose_exp_with_block_size.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll: tiled_transpose_with_loop_unroll.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll.o: tiled_transpose_with_loop_unroll.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_ii: tiled_transpose_with_loop_unroll_parallel_for_ii.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_ii.o: tiled_transpose_with_loop_unroll_parallel_for_ii.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_jj: tiled_transpose_with_loop_unroll_parallel_for_jj.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_jj.o: tiled_transpose_with_loop_unroll_parallel_for_jj.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_i: tiled_transpose_with_loop_unroll_parallel_for_i.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_i.o: tiled_transpose_with_loop_unroll_parallel_for_i.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_j: tiled_transpose_with_loop_unroll_parallel_for_j.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_j.o: tiled_transpose_with_loop_unroll_parallel_for_j.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_1: tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_1.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_1.o: tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_1.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_2: tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_2.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_2.o: tiled_transpose_with_loop_unroll_parallel_for_ii_collapse_2.c
	$(CC) $(CCFLAGS) -c $<

tiled_transpose_with_loop_unroll_loop_count_exp: tiled_transpose_with_loop_unroll_loop_count_exp.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_transpose_with_loop_unroll_loop_count_exp.o: tiled_transpose_with_loop_unroll_loop_count_exp.c
	$(CC) $(CCFLAGS) -c $<

optimized_tiled_transpose: optimized_tiled_transpose.o
	$(CC) $(CCFLAGS) -o $@ $<
optimized_tiled_transpose.o: optimized_tiled_transpose.c
	$(CC) $(CCFLAGS) -c $<
clean:
	rm $(OBJ) $(CMD)
