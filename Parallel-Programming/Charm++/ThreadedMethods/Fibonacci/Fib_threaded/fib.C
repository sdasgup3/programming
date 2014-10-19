#include"fib.decl.h"
#define THRESHOLD 5

   CProxy_Main mainProxy;

class myMsg: public CMessage_myMsg {
  public:
    int val;
};

class Main: public CBase_Main {

  public:
  Main(CkArgMsg *msg) {
    int n = atoi (msg->argv[1]);
    mainProxy = thisProxy;

    CProxy_Worker w = CProxy_Worker::ckNew();
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
  Worker()  {
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

  myMsg* respond(int n) {
    int ret;
    if(n < THRESHOLD) {
      ret = seqFib(n);
    } else {
      CProxy_Worker w1 = CProxy_Worker::ckNew();
      CProxy_Worker w2 = CProxy_Worker::ckNew();
      myMsg* n1 = w1.respond(n-1);
      myMsg* n2 = w1.respond(n-2);
      ret =  n1->val +  n2->val;
      delete n1; delete n2;
    }
    myMsg* msg = new myMsg();
    msg->val = ret;
    return msg;
  }

};


#include"fib.def.h"
