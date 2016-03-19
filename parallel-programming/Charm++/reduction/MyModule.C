#include "stdio.h"
#include "MyModule.decl.h" //based on module name

#define DEFAULT_NUM_ELEMS 10


class Main : public CBase_Main {
  public:
    Main(CkArgMsg* msg) {
      int numElems = msg->argc > 1 ? atoi(msg->argv[1]) :  DEFAULT_NUM_ELEMS;
      CkPrintf("reduction: number of elements = %d\n", numElems);
      CProxy_Elem::ckNew(thisProxy, numElems);
    }
    void printResult(int result) {
      CkPrintf("result = %d\n", result);
      //CkExit();
    }
    void printResult2(CkReductionMsg* result) {
      int * data = (int*)result->getData();
      CkPrintf("result = %d\n", *data);
      CkExit();
    }
};

class Elem : public CBase_Elem {
  public:
    Elem(CProxy_Main mainProxy) {
      int value = thisIndex;
      ckout << "Create "<< thisIndex << endl;
      CkCallback cb(CkReductionTarget(Main, printResult), mainProxy);
      CkCallback cb2(CkIndex_Main::printResult2(NULL), mainProxy);

      contribute(sizeof(int), &value, CkReduction::sum_int, cb);
      contribute(sizeof(int), &value, CkReduction::sum_int, cb2);
    }
    Elem(CkMigrateMessage*) { }
};



#include "MyModule.def.h" //based on module name

