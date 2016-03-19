SHELL=/bin/bash

CC=gcc
CCFLAGS = -O0 -Wall
MPICC=mpicc

CMD = sampleSort psampleSort histogramSort phistogramSort
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

psampleSort: psampleSort.o
	$(MPICC) $(CCFLAGS) -o $@ $<

psampleSort.o: psampleSort.c
	$(MPICC) $(CCFLAGS) -c $<

sampleSort: sampleSort.o
	$(CC) $(CCFLAGS) -o $@ $<

sampleSort.o: sampleSort.c
	$(CC) $(CCFLAGS) -c $<

phistogramSort: phistogramSort.o
	$(MPICC) $(CCFLAGS) -o $@ $<

phistogramSort.o: phistogramSort.c
	$(MPICC) $(CCFLAGS) -c $<

histogramSort: histogramSort.o
	$(CC) $(CCFLAGS) -o $@ $<

histogramSort.o: histogramSort.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm $(OBJ) $(CMD)
