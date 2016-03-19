#include"fib.decl.h"
#define THRESHOLD 5

   CProxy_Main mainProxy;

class Main: public CBase_Main {

  public:
  Main(CkArgMsg *msg) {
    int n = atoi (msg->argv[1]);
    mainProxy = thisProxy;

    CProxy_Worker w = CProxy_Worker::ckNew(1,  CProxy_Worker());
    w.run(n);
  }

  void result(int n) {
    CkPrintf("Result : %d\n", n);
    CkExit();
  }
};



class Worker: public CBase_Worker {
  public:
  CthThread t;  
  int isRoot;
  CProxy_Worker parent;
  int count, total;

  Worker(int a,  CProxy_Worker b) { 
    isRoot = a;
    parent = b; 
      count = total  = 0;
  }

  Worker(CkMigrateMessage* msg) {}

  int seqFib(int n) {
    if(n <=2) {
      return 1;
    } else {
      return seqFib(n-1) + seqFib(n-2);
    }
  }

  void run (int n) {
    t = CthSelf();
    CkPrintf("%d tid %d in  run\n", n, t);
    if(n < THRESHOLD) {
      int ret = seqFib(n);
      if(isRoot) {
        mainProxy.result(ret);
      } else {
        parent.respond(ret);
      }
    } else {

      CkPrintf("%d is spawnung %d and %d\n", n, n-1, n-2);
      CProxy_Worker w1 = CProxy_Worker::ckNew(0,  thisProxy);
      CProxy_Worker w2 = CProxy_Worker::ckNew(0,  thisProxy);
      w1.run(n-1);
      w2.run(n-2);

      CthSuspend();

      CkPrintf("Awaken : total: %d \n", total);
      if(isRoot) {
        mainProxy.result(total);
      } else {
        parent.respond(total);
      }
    }
  }

  void respond(int n) {
    total += n;
    count ++;
    CkPrintf("n: %d count: %d in  respond\n", n, count);
    if(2 == count) {
      CkPrintf("Awakening  %d \n", t);
      CthAwaken(t);
    }
  }
};


#include"fib.def.h"
