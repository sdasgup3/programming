CC   = $(CPP)
CXX   = $(CPP)
HDRS = all_includes_defines.h dfa_utils.h otherFunctions.h \
	bb_cfg_cnode_functions.h globalVarAndStructs.h 	

OBJS = bb_cfg_cnode_functions.o \
	dfa_utils.o             \
	fillTable.o             \
	otherFunctions.o        \
	mainPlugin.o

LANG =
LOGFLAG =
FLAGS= -fPIC -O2

DBGFLAGS=-g -fno-inline
OPTFLAGS=-O3
PROFFLAGS=-DPROFILE

# COMMON FLAGS : Now include -Werror
WARNFLAGS= -Wall -W -Werror
COMFLAGS= -fPIC -shared -ldl  -L/usr/local/lib 
#--DEBUG BUILD
##FLAGS=$(COMFLAGS) $(DBGFLAGS) $(WARNFLAGS)
#--OPT_BUILD
FLAGS=$(COMFLAGS) $(WARNFLAGS) $(OPTFLAGS)


GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include

%.o : %.c $(HDRS)
	echo "A  " $(INCLUDE)
	$(CC) $(LANG) $(LOGFLAG) $(FLAGS) $(INCLUDE) -c $< 

mainPlugin.so: $(OBJS)
	echo "B  " $(INCLUDE)
	$(CC) $(INCLUDE) $(FLAGS) -shared $^ -o $@ 

all:	mainPlugin.so

cc1 : mainPlugin.so
	echo "C " $(INCLUDE)
	$(CC1) -fplugin=./mainPlugin.so ./tcases/test.cpp -fdump-tree-all -fdump-ipa-all -O0

clean:
	-$(RM) -f  *.cpp.* *~  a.out *.c.* *.o
	-$(RM) -f DebugInfo.txt 
	-$(RM) -f shape_bb_dump.txt 
	-$(RM) -f shapeSummary.txt shape.txt

clean_so: clean
	-$(RM) -f mainPlugin.so  mainPlugin.o

fresh: clean_so all
