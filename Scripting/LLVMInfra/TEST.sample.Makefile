##===- TEST.libcalls.Makefile ------------------------------*- Makefile -*-===##
#
# This recursively traverses the programs, and runs the -simplify-libcalls pass
# on each *.linked.rbc bytecode file with -stats set so that it is possible to
# determine which libcalls are being optimized in which programs.
# 
# Usage: 
#     make TEST=libcalls summary (short summary)
#     make TEST=libcalls (detailed list with time passes, etc.)
#     make TEST=libcalls report
#     make TEST=libcalls report.html
#
##===----------------------------------------------------------------------===##

CURDIR  := $(shell cd .; pwd)
PROGDIR := $(PROJ_SRC_ROOT)
RELDIR  := $(subst $(PROGDIR),,$(CURDIR))

# PRE-PASS
OPTS = -mem2reg -loop-rotate -reassociate -lcm -mem2reg -simplifycfg

# BASE-PASS
#OPTS = -mem2reg -loop-rotate -reassociate -simplifycfg

$(PROGRAMS_TO_TEST:%=test.$(TEST).%): \
test.$(TEST).%: Output/%.$(TEST).report.txt
	@cat $<

$(PROGRAMS_TO_TEST:%=Output/%.$(TEST).report.txt):  \
Output/%.$(TEST).report.txt: Output/%.linked.rbc $(LOPT) \
	$(PROJ_SRC_ROOT)/TEST.pre.Makefile 
	$(VERB) $(RM) -f $@
	@echo "---------------------------------------------------------------" >> $@
	@echo ">>> ========= '$(RELDIR)/$*' Program" >> $@
	@echo "---------------------------------------------------------------" >> $@
	@-opt -stats -load /media/Vodka/cs426/unitP/llvm/Debug+Asserts/lib/mp2.so $(OPTS) $< > Output/$*.lcm-opt.bc 2>>$@ 
#summary:
#	@$(MAKE) TEST=libcalls | egrep '======|simplify-libcalls -'

.PHONY: summary
REPORT_DEPENDENCIES := $(LOPT)
