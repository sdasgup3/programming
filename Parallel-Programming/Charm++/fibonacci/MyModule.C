#include "stdio.h"
#include "MyModule.decl.h" //based on module name

#define THRESHOLD 10

/*Main Chare*/
class Master : public CBase_Master {
  public:
  Master(CkArgMsg* m) {
      CProxy_Worker ::ckNew(atoi(m->argv[1]), true, CProxy_Worker());
  };

};

class Worker : public CBase_Worker {
  public:
    CProxy_Worker parent; bool isRoot; int result, count;
    Worker(int n, bool isRoot , CProxy_Worker parent )
        : parent(parent ) , isRoot(isRoot ) , result(0) , count(n < THRESHOLD ? 1 : 2) {
      if (n < THRESHOLD) {
        response(seqFib(n));
      } else {
        CProxy_Worker::ckNew(n - 1, false, thisProxy);
        CProxy_Worker::ckNew(n - 2, false, thisProxy);
      }
    }

  int seqFib(int n) { 
    return n < 2 ? n : seqFib(n - 1) + seqFib(n - 2); 
  }

  void response(int val) {
    result += val;
    if (--count == 0) {
      if (isRoot) {
        CkPrintf("Fibonacci number is: %d\n", result);
        CkExit();
      } else {
        parent.response(result);
        delete this;
      }
    }
  }

};



#include "MyModule.def.h" //based on module name

