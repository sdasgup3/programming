##===- TEST.pre.Makefile ------------------------------*- Makefile -*-===##
##===----------------------------------------------------------------------===##

CURDIR  := $(shell cd .; pwd)
PROGDIR := $(PROJ_SRC_ROOT)
RELDIR  := $(subst $(PROGDIR),,$(CURDIR))
SOFILE  := /home/sdasgup3/llvm/llvm-llvmpa/llvm-build/Release+Asserts/lib/libLLVMLCM.so

$(PROGRAMS_TO_TEST:%=test.$(TEST).%): \
test.$(TEST).%: Output/%.$(TEST).report.txt
	@cat $<

$(PROGRAMS_TO_TEST:%=Output/%.$(TEST).report.txt):  \
Output/%.$(TEST).report.txt: Output/%.linked.rbc $(LOPT) \
	$(PROJ_SRC_ROOT)/TEST.pre.Makefile 
	$(VERB) $(RM) -f $@
	@echo
	@echo
	@echo "---------------------------------------------------------------" >> $@
	@echo ">>> =========OPT '$(RELDIR)/$*' with base" >> $@
	@echo ">>> =========OPT '$(RELDIR)/$*' with lcm" >> $@
	@echo ">>> =========OPT '$(RELDIR)/$* $(LCC)' with gvn" >> $@
	@echo "---------------------------------------------------------------" >> $@
	@-$(LOPT) -load=$(SOFILE) -mem2reg -loop-rotate -reassociate       -mem2reg -simplifycfg -stats Output/$*.linked.rbc  -o Output/$*.base.linked.bc |& tee Output/$*.base.linked.stats
	@-$(LOPT) -load=$(SOFILE) -mem2reg -loop-rotate -reassociate -lcm  -mem2reg -simplifycfg -stats Output/$*.linked.rbc  -o Output/$*.lcm.linked.bc  |& tee Output/$*.lcm.linked.stats
	@-$(LOPT) -load=$(SOFILE) -mem2reg -loop-rotate -reassociate -gvn  -mem2reg -simplifycfg -stats Output/$*.linked.rbc  -o Output/$*.gvn.linked.bc  |& tee Output/$*.gvn.linked.stats 
	@-$(LOPT) -load=$(SOFILE) -mem2reg -loop-rotate -reassociate -lcm  -mem2reg -simplifycfg -mem2reg -loop-rotate -reassociate -gvn  -mem2reg -simplifycfg -stats Output/$*.linked.rbc  -o Output/$*.lcm_gvn.linked.bc  |& tee Output/$*.lcm_gvn.linked.stats
	@-$(LOPT) -load=$(SOFILE) -mem2reg -loop-rotate -reassociate -gvn  -mem2reg -simplifycfg -mem2reg -loop-rotate -reassociate -lcm  -mem2reg -simplifycfg -stats Output/$*.linked.rbc  -o Output/$*.gvn_lcm.linked.bc  |& tee Output/$*.gvn_lcm.linked.stats
	@-$(LCC) -O0 -lm  Output/$*.base.linked.bc 		-o Output/$*.base.linked.exe
	@-$(LCC) -O0 -lm Output/$*.lcm.linked.bc 		-o Output/$*.lcm.linked.exe
	@-$(LCC) -O0 -lm Output/$*.gvn.linked.bc 		-o Output/$*.gvn.linked.exe
	@-$(LCC) -O0 -lm Output/$*.lcm_gvn.linked.bc 	-o Output/$*.lcm_gvn.linked.exe
	@-$(LCC) -O0 -lm Output/$*.gvn_lcm.linked.bc 	-o Output/$*.gvn_lcm.linked.exe


.PHONY: summary
REPORT_DEPENDENCIES := $(LOPT)
