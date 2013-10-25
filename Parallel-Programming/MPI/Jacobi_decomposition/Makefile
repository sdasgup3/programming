SHELL=/bin/bash

CC=gcc
CCFLAGS = -O0
MPICC=mpicc

CMD = jacobi_serial jacobi coljacobi
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

jacobi_serial: jacobi_serial.o
	$(CC) $(CCFLAGS) -o $@ $< -lm

jacobi: jacobi.o
	$(MPICC) $(CCFLAGS) -o $@ $<

coljacobi: coljacobi.o
	$(MPICC) $(CCFLAGS) -o $@ $<

jacobi_serial.o: jacobi_serial.c
	$(CC) $(CCFLAGS) -c $<

jacobi.o: jacobi.c
	$(MPICC) $(CCFLAGS) -c $<

coljacobi.o: coljacobi.c
	$(MPICC) $(CCFLAGS) -c $<

clean:
	rm $(OBJ) $(CMD)
