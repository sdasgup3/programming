TGT=vinay_presentation
# DEPS=   code_treeadd_colored_rw.tex \
# 		defn_fig_0.tex \
# 		defn_fig_1.tex \
# 		properties_fig_1.tex \
# 		motiv_fig_1.tex \
# 		allocation_before.tex \
# 		allocation_after.tex \
# 		fig_ptrassgn_1.tex \
# 		fig_ptrassgn_1_after.tex \
# 		fig_ptrassgn_2.tex \
# 		fig_ptrassgn_2_after.tex \
# 		ptr_strup_1.tex \
# 		ptr_strup_1_after.tex \
# 		ptr_strup_2.tex \
# 		ptr_strup_2_after.tex \
# 		motiv_1_fig_S1.tex \
# 		motiv_1_fig_S3.tex \
# 		motiv_1_fig_S4.tex \
# 		motiv_1_fig_S5.tex \
# 		motiv_1_fig_S6.tex \
# 		motiv_1_fig_S7.tex

LATEX=pslatex
BIBTEX=bibtex

all: $(TGT)
pdf: $(TGT).pdf

$(TGT): $(TGT).pdf

$(TGT).pdf: $(TGT).ps
	ps2pdf $(TGT).ps vinay_presentation.pdf

$(TGT).ps: $(TGT).dvi
	dvips -o $(TGT).ps $(TGT)

$(TGT).dvi: $(TGT).tex #$(DEPS)
	$(LATEX) $(TGT) && $(LATEX) $(TGT)
	#$(LATEX) $(TGT) && $(LATEX) $(TGT)

.PHONY: clean clobber fresh
clean: 
	-$(RM) -f $(TGT).dvi $(TGT).aux $(TGT).log
	-$(RM) -f $(TGT).toc $(TGT).idx $(TGT).bbl
	-$(RM) -f $(TGT).toc $(TGT).ind $(TGT).blg 
	-$(RM) -f $(TGT).ps $(TGT).ind $(TGT).pdf 
	-$(RM) -f $(TGT).snm $(TGT).out $(TGT).nav 
	-$(RM) -f missfont.log *~

clobber: clean
	-$(RM) -f $(TGT).ps vinay_presentation.pdf

fresh: clobber all
