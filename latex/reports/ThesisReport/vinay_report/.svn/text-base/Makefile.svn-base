TGT=Main_Thesis
DEPS=   Chapters/Appendix.tex \
		Chapters/Conclusion.tex \
		Chapters/modified_df_values.tex \
		Chapters/original_df_values.tex \
		Chapters/defn_fig_0.tex \
		Chapters/defn_fig_1.tex \
		Chapters/defn_fig_2.tex \
		Chapters/Interprocedural.tex \
		Chapters/relatedwork_fig_2.tex \
		Chapters/background_fig_1.tex \
		Chapters/Enhancements.tex \
		Chapters/Introduction.tex \
		Chapters/relatedwork_fig_3.tex \
		Chapters/Background.tex \
		Chapters/Implementation.tex \
		Chapters/relatedwork_fig_1.tex \
		Chapters/SubsetBased.tex \
		diagrams/Appendix_2.eps \
		diagrams/Appendix_4.eps \
		diagrams/Diagram1.eps \
		diagrams/Enhancements_d2.eps \
		diagrams/Enhancements_d3.eps \
		diagrams/Impl_2.eps \
		diagrams/Appendix_3.eps \
		diagrams/Diagram12.eps \
		diagrams/Enhancements_d1.eps \
		diagrams/Impl_1.eps \
		diagrams/Impl_3.eps \
		References.bib \
		Chapters/merge_operation.tex \
		Chapters/intersection_operation.tex \
		Chapters/removal_operation.tex


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
	-$(RM) -f $(TGT).lof $(TGT).out 
	-$(RM) -f missfont.log *~

clobber: clean
	-$(RM) -f $(TGT).ps $(TGT).pdf

fresh: clobber all
