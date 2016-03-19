CHARMC=~/charm/net-linux-x86_64/bin/charmc $(OPTS)
TESTOPTS = ++local

OBJS = particle.o

N = 1

all: particle

particle: $(OBJS)
	$(CHARMC) -tracemode projections  -O3 -language charm++ -o particle $(OBJS)

particle.decl.h: particle.ci
	$(CHARMC)  particle.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o particle charmrun

cleanp:
	rm -f *.sts *.gz *.projrc *.topo *.out

particle.o: particle.C particle.decl.h
	$(CHARMC) -c particle.C

test: all
	./charmrun particle $(N) $(TESTOPTS) +p4
