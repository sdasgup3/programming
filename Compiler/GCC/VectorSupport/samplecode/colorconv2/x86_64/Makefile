# /************************************************************************
# *
# *  Filename: Makefile
# *
# *  Description: build the colorconv library for X86 processors
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
# * 11/12/10 gpk added gmon.out to clean target
# * 1/21/10  gpk added target2 for rgbcalc
# * 1/29/11  gpk added target3 for testrange
# *
# ************************************************************************/
SHELL= /bin/sh

MKDEPFILE = Make.depend
MKDEPOPT   = -MD

VPATH = ..

# TARGETn is executable name

TARGET1 = testcolorconv
TARGET2 = rgbcalc
TARGET3 = testrange

TARGETLIB = libcolorconv.a

INCDIRS = -I.. -I.

# CC is name of C compiler
CC = cc
#CC = insure


# SCALAR_OBJS are the non-vector object files

SCALAR_OBJS = cputest.o scalar.o interface.o

# SSE1_OBJS are the SSE1 vector object files

SSE1_OBJS = sse1.o

# SSE1_OBJS are the SSE2 vector object files

SSE2_OBJS = sse2.o

# LIB_OBJS - the object files that make up the $TARGETLIB

LIB_OBJS = ${SCALAR_OBJS} ${SSE1_OBJS}  ${SSE2_OBJS} 

# TARGET1_OBJS - the object files with library test code in them
TARGET1_OBJS = testlib.o

# TARGET1_OBJS - the object files with library test code in them
TARGET2_OBJS = rgbcalc.o

TARGET3_OBJS = testrange.o


# OBJS - all object files
OBJS = ${LIB_OBJS}  ${TARGET1_OBJS}  ${TARGET2_OBJS}  ${TARGET3_OBJS} 

# the source files, scalar, SSE1, SSE2

SCALAR_SRC = ${SCALAR_OBJS:.o=.c}
SSE1_SRC = ${SSE1_OBJS:.o=.c}
SSE2_SRC = ${SSE2_OBJS:.o=.c}

# DUMPS - the .asm dump files of the non-scalar files
# (use this to make sure you're getting the vector 
# instructions you're expecting)

DUMPS=  ${SSE1_OBJS:.o=.asm} ${SSE2_OBJS:.o=.asm}

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

OPTLEVEL = -O3 

# debug settings 
# -fstrict-aliasing
# -march=native
CFLAGS  = -c $(OPTLEVEL) -march=native  -Wall -pedantic $(MKDEPOPT) \
           -malign-double -Wdisabled-optimization -Wcast-align  -Wcast-qual \
             # -std=c99 -std=c90



#CFLAGS  = -c -g -Wall -ansi $(MKDEPOPT) -DTESTCODE # -malign-double

# profiling settings
#CFLAGS  = -c -g -pg -a -Wall -ansi $(MKDEPOPT)


# full optimization settings:
#CFLAGS = -c $(MKDEPOPT)  -O3 -march=pentium3 -mmmx  -msse -mfpmath=sse \
#          -malign-double 


# profiling settings
#LDFLAGS =  -g -pg -a  #  -g -s 

# regular settings

LDFLAGS =  $(OPTLEVEL) -march=native -mmmx  -msse \
            -mfpmath=sse   -malign-double # -s

# -funsafe-loop-optimizations  -fipa-pta -fipa-cp

SSE1_FLAGS =$(CFLAGS) $(DFLAGS) -mmmx -msse -mfpmath=sse\
            -Wunsafe-loop-optimizations -Wunused -Wextra \
	    -funsafe-loop-optimizations    


SSE2_FLAGS = $(SSE1_FLAGS) -msse2 -DUSE_CVT2PS

SCALAR_FLAGS = $(CFLAGS) $(DFLAGS)

all: $(TARGETLIB)  $(TARGET1) $(TARGET2)  $(TARGET3) 

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

sse1.o : sse1.c
	${CC}  ${SSE1_FLAGS}  $<

sse2.o : sse2.c
	${CC}  ${SSE2_FLAGS}  $<


rgbcalc.o : rgbcalc.c
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
	@set -e; ${CC} -MM $(CFLAGS) $(DFLAGS) $(SSE2_FLAGS) $< \
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
	-rm -f *.o *.asm core a.out gmon.out  $(TARGETLIB) *~* *.bak
	-rm $(TARGET1) $(TARGET2)  $(TARGET3) 


cleandepends: clean
	-rm *.d


include $(OBJS:.o=.d)








