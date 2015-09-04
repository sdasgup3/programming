# /************************************************************************
# *
# *  Filename: Makefile
# *
# *  Description: build the colorconv library for powerPC processors 
# *               (use altivec if possible)
# *  
# *  Process:
# *     
# *    make
# *    testcolorconv
# *
# * Revision History
# * 
# * 12/23/09 gpk initial coding
# * 10/15/10 gpk modified for ppc64
# * 11/12/10 gpk added gmon.out to clean target
# * 1/28/10 gpk compile range test and speed test
# * 02/13/11 kaj added PRINTLIB
# * 2/18/11 gpk  put pedantic back in
# * 4/1/11  gpk  moved testrange, rgbcalc to SCALAR_FLAGS instead of 
# *              ALTIVEC_FLAGS
# * 
# ************************************************************************/
SHELL= /bin/sh

MKDEPFILE = Make.depend
MKDEPOPT   = -MD

VPATH = ..

# TARGETn are names of test programs

TARGET1 = testcolorconv
TARGET2 = testrange
TARGET3 = rgbcalc

TARGETLIB = libcolorconv.a
PRINTLIB = libprintValues.a

INCDIRS = -I.. -I.

# CC is name of C compiler
CC = cc
#CC = insure


# SCALAR_OBJS are the non-vector object files

SCALAR_OBJS = cputest.o scalar.o interface.o

# ALTIVEC_OBJS are the altivec-using  object files

ALTIVEC_OBJS = altivec.o



# LIB_OBJS - the object files that make up the $TARGETLIB

LIB_OBJS = ${SCALAR_OBJS} ${ALTIVEC_OBJS} 

# TARGET1n_OBJS - the object files with library test code in them
TARGET1_OBJS = testlib.o
TARGET2_OBJS = testrange.o
TARGET3_OBJS = rgbcalc.o

# OBJS - all object files
OBJS = ${LIB_OBJS} ${TARGET1_OBJS} ${TARGET2_OBJS} ${TARGET3_OBJS}

PRINT_OBJS = printValues.o

# the source files, scalar, ALTIVEC

SCALAR_SRC = ${SCALAR_OBJS:.o=.c}
ALTIVEC_SRC = ${ALTIVEC_OBJS:.o=.c}


# DUMPS - the .asm dump files of the non-scalar files
# (use this to make sure you're getting the vector 
# instructions you're expecting)

DUMPS=  ${ALTIVEC_OBJS:.o=.asm} 

# 
#
# DFLAGS - put all -D defines and -I includes here so lint can use them too.
# CFLAGS - put C compiler specific flags here
#


DFLAGS = ${INCDIRS} -D_BSD_SOURCE 



#
# OPTLEVEL - put optimization related flags here:
# -g for debug (no optimization)
# -O2, -O3 for increasing levels of optimization
# -g -pg for profiling
# -ggdb to make it easier to read the output from objdump
#

OPTLEVEL = -O3 # -ggdb

# debug settings 
# -fstrict-aliasing
# 
CFLAGS  = -c $(OPTLEVEL)  -Wall -Wdisabled-optimization \
          -Wcast-align  -Wcast-qual $(MKDEPOPT) -pedantic 




# profiling settings
#CFLAGS  = -c -g -pg -a -Wall -ansi $(MKDEPOPT) -maltivec -mabi=altivec



# profiling settings
#LDFLAGS =  -g -pg -a  #  -g -s 

# regular settings

LDFLAGS =  $(OPTLEVEL) -maltivec -mabi=altivec # -s

SCALAR_FLAGS = $(CFLAGS) $(DFLAGS) -mtune=powerpc64 -mcpu=powerpc64

ALTIVEC_FLAGS =$(SCALAR_FLAGS)  -maltivec -mabi=altivec



all: $(TARGETLIB) $(PRINTLIB) $(TARGET1) $(TARGET2) $(TARGET3) 

$(TARGETLIB): ${LIB_OBJS}
	ar -rv ${TARGETLIB}  ${LIB_OBJS} 

$(PRINTLIB): $(PRINT_OBJS)
	ar -rv ${PRINTLIB}  ${PRINT_OBJS}

# i hate to have to list these things by file rather than using
# a generic rule, but I can't figure out how to get the compiler
# flags to change by file. anyone know a better way to do this?

cputest.o : cputest.c
	${CC}  ${SCALAR_FLAGS}  $<

scalar.o : scalar.c
	${CC}  ${SCALAR_FLAGS}  $<

interface.o : interface.c
	${CC}  ${SCALAR_FLAGS}  $<

altivec.o : altivec.c
	${CC}  ${ALTIVEC_FLAGS}  $<

testlib.o : testlib.c
	${CC}  ${ALTIVEC_FLAGS}  $<

testrange.o : testrange.c
	${CC}  ${SCALAR_FLAGS}  $<

rgbcalc.o : rgbcalc.c
	${CC}  ${SCALAR_FLAGS}  $<


$(TARGET1):  $(TARGETLIB) ${TARGET1_OBJS}
	${CC} $(LDFLAGS) ${TARGET1_OBJS} -o $@ -L. -lcolorconv -lprintValues

$(TARGET2):  $(TARGETLIB) ${TARGET2_OBJS}
	${CC} $(LDFLAGS) ${TARGET2_OBJS} -o $@ -L. -lcolorconv -lprintValues

$(TARGET3):  $(TARGETLIB) ${TARGET3_OBJS}
	${CC} $(LDFLAGS) ${TARGET3_OBJS} -o $@ -L. -lcolorconv -lprintValues

dumps:${DUMPS}
	echo generating ${DUMPS}

%.asm:%.o
	@echo dumping  $(@F:.asm=.o)
	objdump -dS $(@F:.asm=.o) > $@

%.d:%.c
	@echo Generating dependences for  $(@F:.d=.c)
	@set -e; ${CC} -MM $(CFLAGS) $(DFLAGS) $(ALTIVEC_FLAGS) $< \
                       | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                     [ -s $@ ] || rm -f $@

%.d:%.C
	@echo Generating dependences for  $(@F:.d=.C)
	@set -e; ${CPP} -MM $(CPPFLAGS) $(DFLAGS) $< \
                       | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                     [ -s $@ ] || rm -f $@

%.d:%.cpp
	@echo Generating dependences for  $(@F:.d=.cpp)
	@set -e; ${CPP} -MM $(CPPFLAGS) $(DFLAGS) $< \
                       | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                     [ -s $@ ] || rm -f $@



install:
#   nothing to install



installheaders :
# 	There are no headers to share


clean :
	-rm -f *.o *.asm core a.out $(TARGETLIB) gmon.out *~* *.bak
	-rm $(TARGET1) $(TARGET2)  $(TARGET3)	


cleandepends: clean
	-rm *.d


include $(OBJS:.o=.d)








