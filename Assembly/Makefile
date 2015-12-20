AS=nasm
AS_OPT=-f elf64
CC=gcc

ASM=$(wildcard *.asm)
OBJ=$(ASM:%.asm=%.o)
EXEC=$(ASM:%.asm=%)

all: $(EXEC) $(OBJ)	

%.o: %.asm
	$(AS) $(AS_OPT) -o $@ $<


%:%.o
	$(CC) -o $@ $<

	
maxofthree: maxofthree.o callmaxofthree.c	
	$(CC) $^ -o $@

.PHONY: clean	
clean:
	rm -rf $(OBJ) $(EXEC)

printvars:
	@echo "AS : $(AS)" 
	@echo "CC : $(CC)" 
	@echo "ASM : $(ASM)" 
	@echo "OBJ : $(OBJ)" 
	@echo "OBJ : $(EXEC)" 
