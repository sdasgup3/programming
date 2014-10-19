#include"oddEven.decl.h"
int numelements;
CProxy_Main mainproxy;
CProxy_Worker workerarray;

class myMsg: public CMessage_myMsg {
  public:
    int val;
};

class Main: public CBase_Main {


  public:
    Main(CkArgMsg* msg) {

      if(msg->argc !=2  ) {
        CkExit();
      }
      numelements = atoi(msg->argv[1]);
      mainproxy = thisProxy;
      workerarray = CProxy_Worker::ckNew(numelements);

      workerarray.run();
    }

    void done (CkReductionMsg* msg) {
      for(int i = 0 ;  i < numelements; i++) {
        workerarray(i).dump();
      }
      CkExit();
    }

};

class Worker: public CBase_Worker  {

  public:
    Worker_SDAG_CODE
    int val;
    int i;
    CthThread t;
    Worker() {
      __sdag_init();
      srand(thisIndex);
      val = ((double) rand()/RAND_MAX) * 100;
      CkPrintf("Initial: [%d]: val :  %d\n", thisIndex, val);
    }

    void dump() { 
      CkPrintf("Final: [%d]: val :  %d\n", thisIndex, val);
    }

    Worker(CkMigrateMessage* msg) {}

    void barrier() {
      contribute (CkCallback(CkReductionTarget(Worker,  barrierH), workerarray));
      t = CthSelf();
      CthSuspend();
    } 

    void barrierH() {
      CthAwaken(t);
    } 
};






#include"oddEven.def.h"
