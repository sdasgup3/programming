CC=gcc
IDIR=include
CCFLAGS=-I./$(IDIR)
ODIR=obj

_DEPS=hellomake.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	mkdir -p obj
	$(CC) -c $< -o $@ $(CCFLAGS)

hellomake:: $(OBJ)
	$(CC) $^ $(CCFLAGS) -o $@
hellomake:: $(OBJ)
	echo "multiple targets"  
	
clean: 
	rm -rf $(ODIR)/ *~ core hellomake
