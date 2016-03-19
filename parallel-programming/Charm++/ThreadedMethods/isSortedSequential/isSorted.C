#include "isSorted.decl.h"

CProxy_Main mainProxy;
CProxy_Worker workerarray;
int numchares;

class Main: public CBase_Main {

  public:
  Main(CkArgMsg* msg) {
    if(msg->argc != 2) {
      CkExit();
    }

    mainProxy = thisProxy;
    numchares = atoi(msg->argv[1]);

    workerarray = CProxy_Worker::ckNew(numchares);

    workerarray[0].run();

  }

  void result(bool success) {
    if(success) {
      CkPrintf("success\n");
    } else {
      CkPrintf("failure\n");
    }
    CkExit();
  }
};

class Worker: public CBase_Worker {
  public:
    int val;
    Worker() {
      val =  thisIndex;
    }
    Worker(CkMigrateMessage*) {}

    void run() {
      CkPrintf("[%d] in run\n", thisIndex);
      workerarray[thisIndex + 1].compare(val);
    }
      
    
  void compare(int leftVal) {
    CkPrintf("[%d] in compare\n", thisIndex);
    if(thisIndex == numchares -1 ) {
      mainProxy.result(true);
      return;
    }
    if(leftVal < val) {
      workerarray[thisIndex+1].compare(val);
    } else {
      mainProxy.result(false);  
    }
  }
};



#include "isSorted.def.h"
