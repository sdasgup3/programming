CC=a.o c.a
genprognames=genconfig genflags gencodes genemit genopinit genrecog \
	     genextract genpeep genattr genoutput


# The names of the executable files for those programs.
genprogs=$(genprognames:%=build/%$(build_exeext))

# Object files used in those programs.
genobjnames=$(genprognames:%=%.o) read-rtl.o gensupport.o genattrtab.o \
        genautomata.o gengenrtl.o genmodes.o genpreds.o gengtype.o \
	genconstants.o gen-protos.o scan.o fix-header.o scan-decls.o \
	gencheck.o dummy-conditions.o genconditions.o errors.o ggc-none.o \
	min-insn-modes.o rtl.o print-rtl.o varray.o gcov-iov.o \
	insn-conditions.o gengtype-lex.o gengtype-yacc.o genmddeps.o
genobjnames_bc=$(genprognames:%=%.bc) read-rtl.bc gensupport.bc genattrtab.bc

genobjs_bc=$(genobjnames_bc:%=build/%)


$(genobjs_bc): %.bc :  # dependencies provided by explicit rule later
	echo $(genobjs_bc)

