SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD =  tiled tiled_serial_optimization tiled_parallel_outer_for_i tiled_parallel_outer_for_j tiled_parallel_outer_for_k tiled_parallel_outer_for_ii tiled_parallel_outer_for_jj tiled_parallel_outer_for_kk tiled_collapse_1 tiled_collapse_2 tiled_collapse_3 tiled_block_size_exp optimized_tiled
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)


tiled: tiled.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_serial_optimization: tiled_serial_optimization.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_i: tiled_parallel_outer_for_i.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_j: tiled_parallel_outer_for_j.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_k: tiled_parallel_outer_for_k.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_ii: tiled_parallel_outer_for_ii.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_jj: tiled_parallel_outer_for_jj.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_parallel_outer_for_kk: tiled_parallel_outer_for_kk.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_collapse_1: tiled_collapse_1.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_collapse_2: tiled_collapse_2.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_collapse_3: tiled_collapse_3.o
	$(CC) $(CCFLAGS) -o $@ $<
tiled_block_size_exp: tiled_block_size_exp.o
	$(CC) $(CCFLAGS) -o $@ $<
optimized_tiled: optimized_tiled.o
	$(CC) $(CCFLAGS) -o $@ $<


tiled.o: tiled.c
	$(CC) $(CCFLAGS) -c $<
tiled_serial_optimization.o: tiled_serial_optimization.c
	$(CC) $(CCFLAGS) -c $<
tiled_parallel_outer_for_i.o: tiled_parallel_outer_for_i.c
	$(CC) $(CCFLAGS) -c  $<
tiled_parallel_outer_for_j.o: tiled_parallel_outer_for_j.c
	$(CC) $(CCFLAGS) -c  $<
tiled_parallel_outer_for_k.o: tiled_parallel_outer_for_k.c
	$(CC) $(CCFLAGS) -c  $<
tiled_parallel_outer_for_ii.o: tiled_parallel_outer_for_ii.c
	$(CC) $(CCFLAGS) -c  $<
tiled_parallel_outer_for_jj.o: tiled_parallel_outer_for_jj.c
	$(CC) $(CCFLAGS) -c  $<
tiled_parallel_outer_for_kk.o: tiled_parallel_outer_for_kk.c
	$(CC) $(CCFLAGS) -c  $<
tiled_collapse_1.o: tiled_collapse_1.c
	$(CC) $(CCFLAGS) -c  $<
tiled_collapse_2.o: tiled_collapse_2.c
	$(CC) $(CCFLAGS) -c  $<
tiled_collapse_3.o: tiled_collapse_3.c
	$(CC) $(CCFLAGS) -c  $<
tiled_block_size_exp.o: tiled_block_size_exp.c
	$(CC) $(CCFLAGS) -c  $<
optimized_tiled.o: optimized_tiled.c
	$(CC) $(CCFLAGS) -c  $<

clean:
	rm $(OBJ) $(CMD)

