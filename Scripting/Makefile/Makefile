CC=gcc
IDIR=include
CCFLAGS=-I./$(IDIR)
ODIR=obj

_DEPS=hellomake.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CCFLAGS)

hellomake: $(OBJ)
	$(CC) $^ $(CCFLAGS) -o $@
	
clean: 
	rm -rf $(ODIR)/*.o *~ core hellomake
