SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD =  multiply multiply_forloop_collapse_1 multiply_forloop_collapse_2  multiply_forloop_collapse_3 multiply_parallel_outer_for multiply_parallel_middle_for multiply_parallel_inner_for multiply_loop_interchange_ijk multiply_loop_interchange_ikj multiply_loop_interchange_jik multiply_loop_interchange_jki multiply_loop_interchange_kij multiply_loop_interchange_kji multiply_parallel_middle_for_ver2 optimized_multiply
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

multiply: multiply.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_forloop_collapse_3: multiply_forloop_collapse_3.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_forloop_collapse_2: multiply_forloop_collapse_2.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_forloop_collapse_1: multiply_forloop_collapse_1.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_parallel_outer_for: multiply_parallel_outer_for.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_parallel_middle_for: multiply_parallel_middle_for.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_parallel_inner_for: multiply_parallel_inner_for.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_ijk: multiply_loop_interchange_ijk.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_ikj: multiply_loop_interchange_ikj.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_jik: multiply_loop_interchange_jik.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_jki: multiply_loop_interchange_jki.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_kij: multiply_loop_interchange_kij.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_loop_interchange_kji: multiply_loop_interchange_kji.o
	$(CC) $(CCFLAGS) -o $@ $<
multiply_parallel_middle_for_ver2: multiply_parallel_middle_for_ver2.o
	$(CC) $(CCFLAGS) -o $@ $<
optimized_multiply: optimized_multiply.o
	$(CC) $(CCFLAGS) -o $@ $<

multiply.o: multiply.c
	$(CC) $(CCFLAGS) -c $<
multiply_forloop_collapse_3.o: multiply_forloop_collapse_3.c
	$(CC) $(CCFLAGS) -c $<
multiply_forloop_collapse_2.o: multiply_forloop_collapse_2.c
	$(CC) $(CCFLAGS) -c $<
multiply_forloop_collapse_1.o: multiply_forloop_collapse_1.c
	$(CC) $(CCFLAGS) -c $<
multiply_parallel_outer_for.o: multiply_parallel_outer_for.c
	$(CC) $(CCFLAGS) -c $<
multiply_parallel_middle_for.o: multiply_parallel_middle_for.c
	$(CC) $(CCFLAGS) -c $<
multiply_parallel_inner_for.o: multiply_parallel_inner_for.c
	$(CC) $(CCFLAGS) -c $<
multiply_loop_interchange_ijk.o: multiply_loop_interchange_ijk.c
	$(CC) $(CCFLAGS) -c  $<
multiply_loop_interchange_ikj.o: multiply_loop_interchange_ikj.c
	$(CC) $(CCFLAGS) -c  $<
multiply_loop_interchange_jik.o: multiply_loop_interchange_jik.c
	$(CC) $(CCFLAGS) -c  $<
multiply_loop_interchange_jki.o: multiply_loop_interchange_jki.c
	$(CC) $(CCFLAGS) -c  $<
multiply_loop_interchange_kij.o: multiply_loop_interchange_kij.c
	$(CC) $(CCFLAGS) -c  $<
multiply_loop_interchange_kji.o: multiply_loop_interchange_kji.c
	$(CC) $(CCFLAGS) -c  $<
multiply_parallel_middle_for_ver2.o: multiply_parallel_middle_for_ver2.c
	$(CC) $(CCFLAGS) -c  $<
optimized_multiply.o: optimized_multiply.c
	$(CC) $(CCFLAGS) -c  $<

clean:
	rm $(OBJ) $(CMD)
