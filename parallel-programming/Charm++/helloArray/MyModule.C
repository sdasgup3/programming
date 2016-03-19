#include "arr.decl.h"

int arraySize;
//struct Main : CBase_Main {
class Main : public CBase_Main {
  public:
  Main(CkArgMsg* msg) {
    arraySize = atoi(msg->argv[1]);
    CProxy_hello p = CProxy_hello::ckNew(arraySize);
    p[0].printHello();
  }
};
class hello : public CBase_hello {

  public:
  hello() { }
  hello(CkMigrateMessage*) { }
  void actualPrint(int pe, int id) {
    CkPrintf("PE[%d]: hello from p[%d]\n", pe, id);
  }

  void printHello() {
    thisProxy[thisIndex].actualPrint(CkMyPe(),thisIndex );
    if (thisIndex == arraySize - 1) {
      CkExit();
    } else {
      thisProxy[thisIndex + 1].printHello();
    }
  }
};

#include "arr.def.h"
