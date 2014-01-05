SHELL=/bin/bash

CC=gcc
CCFLAGS =  -O0 -fopenmp

CMD = addition  reduction_2 helloWorld nested_exp_1 nowait max master single atomic reduction workshare
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

max: max.o 
	$(CC) $(CCFLAGS) -o $@ $<
workshare: workshare.o 
	$(CC) $(CCFLAGS) -o $@ $<
reduction: reduction.o 
	$(CC) $(CCFLAGS) -o $@ $<
master: master.o 
	$(CC) $(CCFLAGS) -o $@ $<
single: single.o 
	$(CC) $(CCFLAGS) -o $@ $<
addition: addition.o 
	$(CC) $(CCFLAGS) -o $@ $<
reduction_2: reduction_2.o 
	$(CC) $(CCFLAGS) -o $@ $<
helloWorld: helloWorld.o 
	$(CC) $(CCFLAGS) -o $@ $<
nested_exp_1: nested_exp_1.o 
	$(CC) $(CCFLAGS) -o $@ $<
nowait: nowait.o 
	$(CC) $(CCFLAGS) -o $@ $<
atomic: atomic.o 
	$(CC) $(CCFLAGS) -o $@ $<


addition.o: addition.c 
	$(CC) $(CCFLAGS) -c $<
reduction.o: reduction.c 
	$(CC) $(CCFLAGS) -c $<
master.o: master.c 
	$(CC) $(CCFLAGS) -c $<
single.o: single.c 
	$(CC) $(CCFLAGS) -c $<
max.o: max.c 
	$(CC) $(CCFLAGS) -c $<
reduction_2.o: reduction_2.c 
	$(CC) $(CCFLAGS) -c $<
helloWorld.o: helloWorld.c 
	$(CC) $(CCFLAGS) -c $<
nested_exp_1.o: nested_exp_1.c 
	$(CC) $(CCFLAGS) -c $<
nowait.o: nowait.c 
	$(CC) $(CCFLAGS) -c $<
atomic.o: atomic.c 
	$(CC) $(CCFLAGS) -c $<
workshare.o: workshare.c 
	$(CC) $(CCFLAGS) -c $<

clean:
	rm $(OBJ) $(CMD)
