SHELL=/bin/bash

CC=gcc
CCFLAGS = -O0
MPICC=mpicc

CMD = 1 2 5 6 8 9
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
clean:
	rm $(OBJ) $(CMD)
