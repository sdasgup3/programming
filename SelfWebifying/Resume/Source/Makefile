CMD=pdflatex


all: Resume.pdf Resume_Job.pdf
Resume: Resume.pdf
Resume_Job: Resume_Job.pdf

%.pdf: %.tex
	$(CMD) $<

clean:
	rm -rf *.dvi  *.log *.ps *.aux *.out
