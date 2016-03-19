SHELL=/bin/bash

CC=gcc
CCFLAGS = -O0
MPICC=mpicc

CMD = jacobi pjacobi coljacobi
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

jacobi: jacobi.o
	$(CC) $(CCFLAGS) -o $@ $< -lm

pjacobi: pjacobi.o
	$(MPICC) $(CCFLAGS) -o $@ $<

coljacobi: coljacobi.o
	$(MPICC) $(CCFLAGS) -o $@ $<

jacobi.o: jacobi.c
	$(CC) $(CCFLAGS) -c $<

pjacobi.o: pjacobi.c
	$(MPICC) $(CCFLAGS) -c $<

coljacobi.o: coljacobi.c
	$(MPICC) $(CCFLAGS) -c $<

clean:
	rm $(OBJ) $(CMD)
