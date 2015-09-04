# /************************************************************************
# # *
# # *  Filename: Makefile
# # *
# # *  Description: build the altivec test functions for powerPC processors 
# # *  
# # *  Process:
# # *     
# # *    make
# # *     addsubtract
# # *     char2float
# # *     float2char
# # *     merge
# # *     narrow
# # *	  permute
# # *     select
# # *     shift
# # *     splat
# # *     unpack
# # *     widen
# # *    
# # * Revision History
# # * 
# # * 01/22/11 kaj initial coding
# # * 4/16/11  gpk cleanup
# # *
# # ************************************************************************/
#
SHELL= /bin/sh

# Where to look for code
VPATH = ..

# apps to compile
TARGET_SHORT_NAMES = addsub char2float float2char merge narrow permute select \
                     shift splat unpack widen 
TARGETS = altivec_add_subtract altivec_char2float altivec_float2char \
          altivec_merge altivec_narrow altivec_permute altivec_select \
          altivec_shift altivec_splat altivec_unpack altivec_widen

# C is name of C compiler
CC = cc
#CC = insure

#
# OPTLEVEL - put optimization related flags here:
# -g for debug (no optimization)
#  # -O2, -O3 for increasing levels of optimization
# -g -pg for profiling
OPTLEVEL = -g #-O3 -ggdb

CFLAGS  = $(OPTLEVEL)  -maltivec -mabi=altivec


all: $(TARGET_SHORT_NAMES)

addsub:
	$(CC) $(CFLAGS) -o altivec_add_subtract altivec_add_subtract.c 

char2float:
	$(CC) $(CFLAGS) -o altivec_char2float altivec_char2float.c 

float2char:
	$(CC) $(CFLAGS) -o altivec_float2char altivec_float2char.c 

merge:
	$(CC) $(CFLAGS) -o altivec_merge altivec_merge.c 

narrow:
	$(CC) $(CFLAGS) -o altivec_narrow altivec_narrow.c 

permute:
	$(CC) $(CFLAGS) -o altivec_permute altivec_permute.c 

select:
	$(CC) $(CFLAGS) -o altivec_select altivec_select.c 

shift:
	$(CC) $(CFLAGS) -o altivec_shift altivec_shift.c 

splat:
	$(CC) $(CFLAGS) -o altivec_splat altivec_splat.c 

unpack:
	$(CC) $(CFLAGS) -o altivec_unpack altivec_unpack.c 

widen:
	$(CC) $(CFLAGS) -o altivec_widen altivec_widen.c 

clean:
	-rm -f *.o *.asm core a.out $(TARGETS) $(PRINTLIB) *~* *.bak


