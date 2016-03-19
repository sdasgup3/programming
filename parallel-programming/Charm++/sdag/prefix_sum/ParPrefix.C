#include <stdio.h>
#include <math.h>
#include "prefixSum.decl.h" //based on module name

/*readonly*/ CProxy_Main mainProxy;

class Main : public CBase_Main {
  public:
  Main(CkArgMsg* msg) {
    mainProxy= thisProxy;
    int numElements;
    if (msg->argc > 1) 
      numElements = atoi(msg->argv[1]);
    numElements = 8; // Default numElements to 8
    delete msg;
              
    CProxy_PrefixSum proxy = CProxy_PrefixSum::ckNew(numElements, numElements);
    proxy.start();
  }
  Main(CkMigrateMessage* msg) {}
  void done() { 
    CkExit(); 
  }
};

class PrefixSum : public CBase_PrefixSum {
  private:
    int N, myVal, dist;
  public:
    PrefixSum_SDAG_CODE
    PrefixSum(int length) : N(length), myVal(1) {
      __sdag_init();
    }
    PrefixSum (CkMigrateMessage*) {};
};

#include "prefixSum.def.h" //based on module name
