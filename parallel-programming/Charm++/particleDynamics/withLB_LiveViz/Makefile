OPTS = -module liveViz -module RefineLB -module GreedyLB -module GreedyCommLB
#CHARMC = ~/charm/net-linux-x86_64/bin/charmc $(OPTS)
CHARMC = /home/acun2/charm/net-linux-x86_64/bin/charmc   $(OPTS)
#CHARMC = /home/sdasgup3/CS598LVK/charm/net-linux-x86_64/bin/charmc -module liveViz  $(OPTS)

TESTOPTS = ++local

OBJS = particle.o

N = 10000
K = 16
#N = 10
#K = 40

all: particle

particle: $(OBJS)
	$(CHARMC) -O3  -language charm++ -o particle $(OBJS)

particle.decl.h: particle.ci
	$(CHARMC)  particle.ci

clean:
	rm -f *.decl.h *.def.h conv-host *.o particle charmrun

cleanp:
	rm -f *.sts *.gz *.projrc *.topo *.out numt.* charmrun.*

particle.o: particle.C particle.decl.h
	$(CHARMC) -c particle.C

test: all
	./charmrun particle $(N) $(K) $(TESTOPTS) +p4

server: all
	./charmrun ./particle $(N) $(K) $(TESTOPTS) +p4 ++server ++server-port 1234

#cd tmp/libs/ck-libs/liveViz ; make
#git clone http://charm.cs.uiuc.edu/gerrit/ccs_tools        
#~/charm/ccs_tools/bin/liveViz localhost 1234        
