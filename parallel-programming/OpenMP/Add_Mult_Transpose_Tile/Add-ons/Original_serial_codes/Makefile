SHELL=/bin/bash

CC=gcc
CCFLAGS = -Wall -O0 -fopenmp

CMD = transpose multiply addition tiled
SRC = $(CMD:=.c)
OBJ = $(SRC:.c=.o) 

all: $(CMD)

transpose: transpose.o
	$(CC) $(CCFLAGS) -o $@ $<

multiply: multiply.o
	$(CC) $(CCFLAGS) -o $@ $<

addition: addition.o
	$(CC) $(CCFLAGS) -o $@ $<

tiled: tiled.o
	$(CC) $(CCFLAGS) -o $@ $<

transpose.o: transpose.c
	$(CC) $(CCFLAGS) -c $<

multiply.o: multiply.c
	$(CC) $(CCFLAGS) -c $<

addition.o: addition.c
	$(CC) $(CCFLAGS) -c $<

tiled.o: tiled.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm $(OBJ) $(CMD)
