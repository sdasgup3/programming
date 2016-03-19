NASM=nasm
AS=as
NASM_OPT=-f elf64
AS_OPT=--gstabs+


CC=gcc

ASM_NASM=$(wildcard *.asm)
ASM_AS=$(wildcard *.s)

ASM=$(ASM_NASM) $(ASM_AS)  

OBJ_I=$(ASM:%.asm=%.o) 
OBJ=$(OBJ_I:%.s=%.o) 

EXEC=$(OBJ:%.o=%)

all: $(EXEC) $(OBJ)	
%:%.s  

%.o: %.asm
	$(NASM) $(NASM_OPT) -o $@ $<

%: %.s
	$(AS) $(AS_OPT) -o $@.o $<
	ld $@.o	-o $@

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
	@echo "EXEC : $(EXEC)" 
