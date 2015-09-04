# /************************************************************************
# *
# *  Filename: Makefile
# *
# *  Description: build the colorconv library for ARM processors
# *  
# *  Process:
# *     
# *    make
# *    testcolorconv
# *
# *  by default this will build at -O3.
# * 
# *  to build with debugging, override the value of OPTLEVEL from the 
# *     command line:
# *
# *    make OPTLEVEL="-g -pg"                  for profiling
# *    make OPTLEVEL=-g                        for debugging
# *
# * Revision History
# * 
# * 12/23/09 gpk initial coding
# * 3/12/11  gpk added targets for rgbcalc, testrange
# * 3/27/11  gpk added SMALL_MEMORY to DFLAGS
# * 4/1/11   gpk move rgbcalc, testrange to SCALAR_FLAGS instead of NEON_FLAGS
# *              add -mcpu=cortex-a8 to SCALAR_FLAGS
# *
# ************************************************************************/
SHELL= /bin/sh

MKDEPFILE = Make.depend
MKDEPOPT   = -MD

VPATH = ..

# TARGET is executable name

TARGET1 = testcolorconv
TARGET2 = rgbcalc
TARGET3 = testrange
TARGETLIB = libcolorconv.a

INCDIRS = -I.. -I.

# CC is name of C compiler
CC = cc



# SCALAR_OBJS are the non-vector object files

SCALAR_OBJS = cputest.o scalar.o interface.o

NEON_OBJS = neon.o 

# LIB_OBJS - the object files that make up the $TARGETLIB

LIB_OBJS = ${SCALAR_OBJS} ${NEON_OBJS}

# TARGET1_OBJS - the object files with library test code in them
TARGET1_OBJS = testlib.o

# TARGET2_OBJS - the object files with library test code in them
TARGET2_OBJS = rgbcalc.o

TARGET3_OBJS = testrange.o

# TEST_OBJS - the object files with library test code in them
TEST_OBJS = testlib.o

# OBJS - all object files
OBJS = ${LIB_OBJS}  ${TEST_OBJS} ${TARGET1_OBJS}  ${TARGET2_OBJS} \
       ${TARGET3_OBJS} 

# the source files, scalar

SCALAR_SRC = ${SCALAR_OBJS:.o=.c}
NEON_SRC = ${NEON_OBJS:.o=.c}


# DUMPS - the .asm dump files of the non-scalar files
# (use this to make sure you're getting the vector 
# instructions you're expecting)

DUMPS=  ${NEON_OBJS:.o=.asm} 

# 
#
# DFLAGS - put all -D defines and -I includes here so lint can use them too.
# CFLAGS - put C compiler specific flags here
#


DFLAGS = ${INCDIRS} -D_BSD_SOURCE -DSMALL_MEMORY



#
# OPTLEVEL - put optimization related flags here:
# -g for debug (no optimization)
# -O2, -O3 for increasing levels of optimization
# -g -pg for profiling
# -ggdb to add information that makes objdump easier to read

OPTLEVEL = -O3 # -ggdb

# debug settings 
# -fstrict-aliasing
# -march=pentium3
CFLAGS = -c $(OPTLEVEL)  -std=c99



# profiling settings
#LDFLAGS =  -g -pg -a  #  -g -s 

# regular settings

LDFLAGS =  $(OPTLEVEL) # -s

SCALAR_FLAGS = $(CFLAGS) $(DFLAGS) -mcpu=cortex-a8 

NEON_FLAGS = $(SCALAR_FLAGS)  -mfpu=neon -mfloat-abi=softfp 


all: $(TARGETLIB)   $(TARGET1) $(TARGET2)  $(TARGET3) 

$(TARGETLIB): ${LIB_OBJS}
	ar -rv ${TARGETLIB}  ${LIB_OBJS} 

# i hate to have to list these things by file rather than using
# a generic rule, but I can't figure out how to get the compiler
# flags to change by file. anyone know a better way to do this?

cputest.o : cputest.c
	${CC}  ${SCALAR_FLAGS}  $<

scalar.o : scalar.c
	${CC}  ${SCALAR_FLAGS}  $<

interface.o : interface.c
	${CC}  ${SCALAR_FLAGS}  $<

neon.o : neon.c
	${CC}  ${NEON_FLAGS}  $<

testlib.o : testlib.c
	${CC}  ${NEON_FLAGS}  $<

rgbcalc.o: rgbcalc.c
	${CC}  ${SCALAR_FLAGS}  $<

testrange.o : testrange.c
	${CC}  ${SCALAR_FLAGS}  $<

$(TARGET1):  $(TARGETLIB) ${TARGET1_OBJS}
	${CC} $(LDFLAGS) ${TARGET1_OBJS} -o $@ -L. -lcolorconv  

$(TARGET2):  $(TARGETLIB) ${TARGET2_OBJS}
	${CC} $(LDFLAGS) ${TARGET2_OBJS} -o $@ -L. -lcolorconv  

$(TARGET3):  $(TARGETLIB) ${TARGET3_OBJS}
	${CC} $(LDFLAGS) ${TARGET3_OBJS} -o $@ -L. -lcolorconv  

dumps:${DUMPS}
	echo generating ${DUMPS}

%.asm:%.o
	@echo dumping  $(@F:.asm=.o)
	objdump -dS $(@F:.asm=.o) > $@

%.d:%.c
	@echo Generating dependences for  $(@F:.d=.c)
	@set -e; ${CC} -MM $(CFLAGS) $(DFLAGS) $(NEON_FLAGS) $< \
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
	-rm -f *.o *.asm core a.out $(TARGET) $(TARGETLIB) *~* *.bak


cleandepends: clean
	-rm *.d


include $(OBJS:.o=.d)








