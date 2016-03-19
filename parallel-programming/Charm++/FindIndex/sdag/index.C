#include "index.decl.h"
#define PER_B_ELEMS 10 
#define PER_A_ELEMS 5 
CProxy_Main mainProxy;
CProxy_B BProxy;
CProxy_A AProxy;

class myMsg: public CMessage_myMsg {
  public:
  int val;
};

class Main: public CBase_Main {

  public:
  Main(CkArgMsg* msg) {

    int n = atoi(msg->argv[1]);
    mainProxy = thisProxy;
    
    CkPrintf("n = %d\n",n);
    BProxy = CProxy_B::ckNew(n);
    AProxy = CProxy_A::ckNew(2);
    AProxy.F();

    CkCallback cb = CkCallback(CkReductionTarget(Main, done), thisProxy);
    CkStartQD(cb);

  }

  void done() {
        CkPrintf("Quiescence2 detecetd\n");
        CkExit();
  }

};


class B: public CBase_B {
  public:
    B(CkMigrateMessage* msg) {}
    int GlobArr[PER_B_ELEMS];
    B() {
      for(int i =0 ; i < PER_B_ELEMS ; i++) {
        GlobArr[i] = i + thisIndex*10;
      }

      //CkPrintf("B[%d] Contents: ", thisIndex);
      for(int i = 0 ; i < PER_B_ELEMS; i ++) {
       // CkPrintf("%d ", GlobArr[i]);
      }
      CkPrintf("\n");
    }

    void G(int n, int parent, int index) {
      AProxy[parent].H(index, GlobArr[n%PER_B_ELEMS]);
    }

};


class A: public CBase_A {
  public:
    A_SDAG_CODE
    int i;
    A(CkMigrateMessage* msg) {}
    int Q[PER_A_ELEMS];
    int T[PER_A_ELEMS];
    A() {
      __sdag_init();

      for(int i =0 ; i < PER_A_ELEMS ; i++) {
        Q[i] = i*PER_B_ELEMS + i; // 0 11 22 33 44
      }
      CkPrintf("Q[%d] Contents\n", thisIndex);
      for(int i = 0 ; i < PER_A_ELEMS; i ++) {
        CkPrintf("%d ", Q[i]);
      }
      CkPrintf("\n");
    }

};




#include "index.def.h"
