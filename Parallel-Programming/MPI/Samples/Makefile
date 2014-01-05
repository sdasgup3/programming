SHELL=/bin/bash

CC=gcc
CCFLAGS = -O0
MPICC=mpicc

CMD = 1 2 5 6 8 9 fairness all_gather reduce prefix_sum exm_1 exm_2  samplesort
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

1: 1.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
1.o: 1.c
	$(MPICC) $(CCFLAGS) -c $<
2: 2.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
2.o: 2.c
	$(MPICC) $(CCFLAGS) -c $<
5: 5.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
5.o: 5.c
	$(MPICC) $(CCFLAGS) -c $<
6: 6.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
6.o: 6.c
	$(MPICC) $(CCFLAGS) -c $<
8: 8.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
8.o: 8.c
	$(MPICC) $(CCFLAGS) -c $<
9: 9.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
9.o: 9.c
	$(MPICC) $(CCFLAGS) -c $<
fairness: fairness.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
fairness.o: fairness.c
	$(MPICC) $(CCFLAGS) -c $<
all_gather: all_gather.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
all_gather.o: all_gather.c
	$(MPICC) $(CCFLAGS) -c $<
reduce: reduce.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
reduce.o: reduce.c
	$(MPICC) $(CCFLAGS) -c $<
prefix_sum: prefix_sum.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
prefix_sum.o: prefix_sum.c
	$(MPICC) $(CCFLAGS) -c $<
exm_1: exm_1.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
exm_1.o: exm_1.c
	$(MPICC) $(CCFLAGS) -c $<
exm_2: exm_2.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
exm_2.o: exm_2.c
	$(MPICC) $(CCFLAGS) -c $<
samplesort: samplesort.o
	$(MPICC) $(CCFLAGS) -o $@ $< -lm
samplesort.o: samplesort.c
	$(MPICC) $(CCFLAGS) -c $<
clean:
	rm $(OBJ) $(CMD)
