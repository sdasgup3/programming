TGT=isse
DEPS=isse.bib \
	Sections/Abstract.tex \
	Sections/Introduction.tex \
	Sections/RelatedWork.tex \
	Sections/MotivatingExample.tex \
	Sections/Definition.tex \
	Figures/Fig_defn_1.tex \
	Figures/Fig_defn_2.tex \
	Figures/Fig_defn_3.tex \
	Sections/Analysis.tex \
	Sections/SubsetBasedAnalysis.tex \
	Sections/ShapeBasedAnalysis.tex \
	Sections/Properties.tex \
	Sections/Implementation.tex \
	Sections/Conclusion.tex  



LATEX=latex
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
	-$(RM) $(TGT).dvi $(TGT).aux $(TGT).log
	-$(RM) $(TGT).toc $(TGT).idx $(TGT).bbl
	-$(RM) $(TGT).toc $(TGT).ind $(TGT).blg 
	-$(RM) $(TGT).ind
	-$(RM) missfont.log *~

clobber: clean
	-$(RM) $(TGT).ps $(TGT).pdf

fresh: clobber all
