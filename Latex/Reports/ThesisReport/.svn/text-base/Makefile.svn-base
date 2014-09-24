TGT=Report
DEPS=   abstract_matter.tex \
		acknow.tex \
		Declaration.tex \
		Appendix.tex \
		RelatedWork.tex \
		Introduction.tex \
		Motivating.tex \
		Conclusion.tex \
		Definitions.tex \
		FutureWork.tex \
		Analysis.tex \
		Properties.tex \
		Comparison.tex \
		test_1.tex \
		test_2.tex \
		test_3.tex \
		motiv_fig_1.tex \
		motiv_fig_2.tex \
		test_3.tex \
		defn_fig_0.tex \
		defn_fig_1.tex \
		defn_fig_2.tex \
		relatedwork_fig_1.tex \
		relatedwork_fig_2.tex \
		relatedwork_fig_3.tex \
		Report.bib

LATEX=pslatex
BIBTEX=bibtex

all: $(TGT)
pdf: $(TGT).pdf

$(TGT): $(TGT).pdf

$(TGT).pdf: $(TGT).ps
	ps2pdf $(TGT).ps $(TGT).pdf

$(TGT).ps: $(TGT).dvi
	dvips -o $(TGT).ps $(TGT)

$(TGT).dvi: $(TGT).tex $(DEPS)
	$(LATEX) $(TGT) && $(LATEX) $(TGT)
	$(BIBTEX) $(TGT)
	$(LATEX) $(TGT) && $(LATEX) $(TGT)

.PHONY: clean clobber fresh
clean: 
	-$(RM) -f $(TGT).dvi $(TGT).aux $(TGT).log
	-$(RM) -f $(TGT).toc $(TGT).idx $(TGT).bbl
	-$(RM) -f $(TGT).toc $(TGT).ind $(TGT).blg 
	-$(RM) -f $(TGT).ps $(TGT).ind $(TGT).pdf 
	-$(RM) -f missfont.log *~

clobber: clean
	-$(RM) -f $(TGT).ps $(TGT).pdf

fresh: clobber all
