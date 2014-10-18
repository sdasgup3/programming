#include"fib.decl.h"
#define THRESHOLD 5

   CProxy_Main mainProxy;

class Main: public CBase_Main {

  public:
  Main(CkArgMsg *msg) {
    int n = atoi (msg->argv[1]);
    mainProxy = thisProxy;

    CProxy_Worker w = CProxy_Worker::ckNew(1, CProxy_Worker());
    w.run(n);
  }

  void result(int n) {
    CkPrintf("Result : %d\n", n);
    CkExit();
  }
};



class Worker: public CBase_Worker {
  Worker_SDAG_CODE
  public:
    CProxy_Worker parent;
    int isRoot;
  Worker(int isRoot, CProxy_Worker p):isRoot(isRoot), parent(p)  {
    __sdag_init();
  }

  Worker(CkMigrateMessage* msg) {}

  int seqFib(int n) {
    if(n <=2) {
      return 1;
    } else {
      return seqFib(n-1) + seqFib(n-2);
    }
  }

};


#include"fib.def.h"
