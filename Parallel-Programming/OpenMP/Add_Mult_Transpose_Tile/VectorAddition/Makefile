SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD = addition addition_parallel_for addition_parallel_for_nowait addition_parallel_for_schedule_static_exp1 addition_parallel_for_schedule_static_exp2 addition_parallel_for_schedule_dynamic_chunk_10000 addition_parallel_for_schedule_dynamic_chunk_1000 addition_parallel_for_schedule_dynamic_chunk_100 addition_parallel_for_schedule_dynamic_chunk_1 addition_parallel_for_schedule_guided optimized_addition
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

addition: addition.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for: addition_parallel_for.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_nowait: addition_parallel_for_nowait.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_static_exp1: addition_parallel_for_schedule_static_exp1.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_static_exp2: addition_parallel_for_schedule_static_exp2.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_dynamic_chunk_10000: addition_parallel_for_schedule_dynamic_chunk_10000.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_dynamic_chunk_1000: addition_parallel_for_schedule_dynamic_chunk_1000.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_dynamic_chunk_100: addition_parallel_for_schedule_dynamic_chunk_100.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_dynamic_chunk_1: addition_parallel_for_schedule_dynamic_chunk_1.o
	$(CC) $(CCFLAGS) -o $@ $<
addition_parallel_for_schedule_guided: addition_parallel_for_schedule_guided.o
	$(CC) $(CCFLAGS) -o $@ $<
optimized_addition: optimized_addition.o
	$(CC) $(CCFLAGS) -o $@ $<


addition.o: addition.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for.o: addition_parallel_for.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_nowait.o: addition_parallel_for_nowait.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_static_exp1.o: addition_parallel_for_schedule_static_exp1.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_static_exp2.o: addition_parallel_for_schedule_static_exp2.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_dynamic_chunk_10000.o: addition_parallel_for_schedule_dynamic_chunk_10000.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_dynamic_chunk_1000.o: addition_parallel_for_schedule_dynamic_chunk_1000.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_dynamic_chunk_100.o: addition_parallel_for_schedule_dynamic_chunk_100.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_dynamic_chunk_1.o: addition_parallel_for_schedule_dynamic_chunk_1.c
	$(CC) $(CCFLAGS) -c $<
addition_parallel_for_schedule_guided.o: addition_parallel_for_schedule_guided.c
	$(CC) $(CCFLAGS) -c $<
optimized_addition.o: optimized_addition.c
	$(CC) $(CCFLAGS) -c $<


clean:
	rm $(OBJ) $(CMD)
