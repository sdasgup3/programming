#include "stdio.h"
#include "MyModule.decl.h" //based on module name

#define THRESHOLD 10

/*Main Chare*/
class Master : public CBase_Master {
  public:
  Master(CkArgMsg* m) {
      CProxy_Fib ::ckNew(atoi(m->argv[1]), true, CProxy_Fib());
  };
};

class Fib : public CBase_Fib {
  public:
    Fib_SDAG_CODE
    CProxy_Fib parent; bool isRoot;

    Fib(int n, bool isRoot , CProxy_Fib parent )
        : parent(parent ) , isRoot(isRoot ) {
      __sdag_init();
      calc(n);
    }

  int seqFib(int n) { 
    return n < 2 ? n : seqFib(n - 1) + seqFib(n - 2); 
  }

  void respond(int val) {
      if (isRoot) {
        CkPrintf("Fibonacci number is: %d\n", val);
        CkExit();
      } else {
        parent.response(val);
        delete this;
      }
  }
};



#include "MyModule.def.h" //based on module name

