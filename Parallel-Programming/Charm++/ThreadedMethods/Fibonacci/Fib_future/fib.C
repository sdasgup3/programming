#include"fib.decl.h"
#define THRESHOLD 5

   CProxy_Main mainProxy;

class myMsg: public CMessage_myMsg {
  public:
    int val;

};

class Main: public CBase_Main {

  public:
  Main(CkMigrateMessage* msg) {}  
  Main(CkArgMsg *msg) {
    int n = atoi (msg->argv[1]);
    CkPrintf("n = %d\n", n);
    mainProxy = thisProxy;
    thisProxy.run(n);
  }

  void run(int n) {

    CkFuture f= CkCreateFuture();

    CProxy_Worker w = CProxy_Worker::ckNew(n, f);
    //w.run(n, f);

    myMsg* m = (myMsg *)CkWaitFuture(f);
    CkPrintf("Result : %d\n", m->val);
    CkExit();
    delete m;
  }

};

class Worker: public CBase_Worker {
  public:

  Worker(int n, CkFuture f) { thisProxy.run(n,f); }

  Worker(CkMigrateMessage* msg) {}

  int seqFib(int n) {
    if(n <=2) {
      return 1;
    } else {
      return seqFib(n-1) + seqFib(n-2);
    }
  }

  void run (int n, CkFuture f) {
    int ret;
    if(n < THRESHOLD) {
      ret = seqFib(n);
    } else {

      CkFuture f1 = CkCreateFuture();
      CkFuture f2 = CkCreateFuture();

      CProxy_Worker w1 = CProxy_Worker::ckNew(n-1,f1);
      CProxy_Worker w2 = CProxy_Worker::ckNew(n-2, f2);
      //w1.run(n-1, f1);
     // w2.run(n-2, f2);

      myMsg* msg1 = (myMsg *)CkWaitFuture(f1);
      myMsg* msg2 = (myMsg *)CkWaitFuture(f2);

      ret = msg1->val + msg2->val;
      delete msg1;
      delete msg2;
    }
    CkPrintf("Returned Value %d\n", ret);
    myMsg* msg = new myMsg();
    msg->val = ret;
    CkSendToFuture(f,msg);
  }

};


#include"fib.def.h"
