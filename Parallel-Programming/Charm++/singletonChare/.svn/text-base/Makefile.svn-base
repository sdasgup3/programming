#CHARMC=/home/acun2/charm/net-darwin-x86_64/bin/charmc $(OPTS)
CHARMC=/Users/bilge/charm_no_touch/net-darwin-x86_64/bin/charmc $(OPTS)

all: arrayHello

arrayHello:   arrayHello.o
	$(CHARMC) -tracemode projections arrayHello.o -o arrayHello -language charm++ 

arrayHello.o : arrayHello.C arrayHello.def.h arrayHello.decl.h
	$(CHARMC) -I../../../src/charj/libs/ -c arrayHello.C

arrayHello.def.h arrayHello.decl.h : arrayHello.ci
	$(CHARMC) arrayHello.ci

test: all
	./charmrun arrayHello +p4 4 ++local

clean:
	rm -f conv-host *.o charmrun arrayHello
	rm -f *.def.h *.decl.h
	rm -f *.log.gz *.projrc *.topo *.sts

