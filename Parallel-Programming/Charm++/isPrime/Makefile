#CHARMC=/home/acun2/charm/net-linux-x86_64/bin/charmc $(OPTS)
CHARMC=/home/acun2/charm/net-linux-x86_64/bin/charmc $(OPTS)

M = 1000
k = 100

all: prime

prime:   prime.o
	$(CHARMC) -tracemode projections prime.o -o prime -language charm++ 

prime.o : prime.C prime.def.h prime.decl.h
	$(CHARMC) -c prime.C

prime.def.h prime.decl.h : prime.ci
	$(CHARMC) prime.ci

test: all
	charmrun prime $(M) $(k) +p4 ++local

clean:
	rm -f conv-host *.o charmrun prime
	rm -f *.def.h *.decl.h
	rm -f *.log.gz *.projrc *.topo *.sts

