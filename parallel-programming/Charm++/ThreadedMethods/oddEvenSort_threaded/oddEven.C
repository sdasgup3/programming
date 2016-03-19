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
    int val;
    CthThread t;
    Worker() {
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


    void run() {
      for (int i = 0 ; i < numelements; i++) {
        
        if(thisIndex % 2 == 0 && thisIndex != numelements -1 ) {
          myMsg* m = workerarray[thisIndex + 1].sendSmaller(val);
          val = m->val;
          delete m;
        } 
        barrier();
        if (thisIndex % 2 == 1 && thisIndex != numelements -1 ) {
          myMsg* m  = workerarray[thisIndex + 1].sendSmaller(val);
          val = m->val;
          delete m;
        }
        barrier();
      }

      contribute(CkCallback(CkIndex_Main::done(NULL), mainproxy)); 

    }

    myMsg* sendSmaller(int leftVal) {
      myMsg* msg = new myMsg();
      if(leftVal > val) {
        msg->val  = val;
        val = leftVal;
      } else {
        msg->val  = leftVal;
      }
      return msg;
    }
    


};






#include"oddEven.def.h"
