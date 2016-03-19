#include "isSorted.decl.h"
#include"pup_stl.h"

CProxy_Main mainProxy;
CProxy_Worker workerarray;
int numchares;

#define wrap(x) ((x + numchares) % numchares)   

class myMsg: public CMessage_myMsg {
  public:
    int val;
};

class Main: public CBase_Main {

  public:
  Main(CkArgMsg* msg) {
    if(msg->argc != 2) {
      CkExit();
    }

    mainProxy = thisProxy;
    numchares = atoi(msg->argv[1]);

    workerarray = CProxy_Worker::ckNew(numchares);

    CkCallback *cb = new CkCallback(CkReductionTarget(Main,done), thisProxy);
    workerarray.ckSetReductionClient(cb);

    workerarray.run();

  }

  void result(bool success) {
    if(success) {
      CkPrintf("success\n");
    } else {
      CkPrintf("failure\n");
    }
    CkExit();
  }

  void done (bool isSorted) {
    CkPrintf("Status: %d\n\n", isSorted);
    CkExit();
  }
};

class Worker: public CBase_Worker {
  Worker_SDAG_CODE
  public:
    int val;
    Worker() {
      __sdag_init();
      val =  thisIndex;
    }

    void pup(PUP::er &p) {
      CBase_Worker::pup(p);
      __sdag_pup(p);
      p|val;
    }

    Worker(CkMigrateMessage*) {}
    myMsg* getValue() {
      myMsg* ret = new myMsg();
      ret->val = val;
      return ret;
    }
};



#include "isSorted.def.h"
