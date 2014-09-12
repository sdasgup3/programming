#CHARMC=/home/acun2/charm/net-linux-x86_64/bin/charmc $(OPTS)
CHARMC=charmc $(OPTS)
TEST=unifyChareLoad

all: $(TEST)

$(TEST):   $(TEST).o
	$(CHARMC) -tracemode projections $(TEST).o -o $(TEST) -language charm++ 

$(TEST).o : $(TEST).C $(TEST).def.h $(TEST).decl.h
	$(CHARMC) -I../../../src/charj/libs/ -c $(TEST).C

$(TEST).def.h $(TEST).decl.h : $(TEST).ci
	$(CHARMC) $(TEST).ci

test: all
#charmrun +p4 $(TEST) 1024 5000 30000  ++local
#charmrun +p4  $(TEST) 4 0 10 9 3 4 3  ++local
	charmrun +p4  $(TEST) 8 0 10   ++local

clean:
	rm -f conv-host *.o charmrun $(TEST)
	rm -f *.def.h *.decl.h
	rm -f *.log.gz *.projrc *.topo *.sts log

