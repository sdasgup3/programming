#include "arr.decl.h"

/*readonly*/ int arraySize;

struct Main : CBase_Main {
  Main(CkArgMsg* msg) {
    arraySize = atoi(msg->argv[1]);
    CProxy_hello proxy = CProxy_hello::ckNew(arraySize);
    proxy[0].printHello();
  }
};

struct hello : CBase_hello {
  hello() { }
  hello(CkMigrateMessage*) { }
  void printHello() {
    CkPrintf("%d: hello from %d\n", CkMyPe(), thisIndex);
    if (thisIndex == arraySize - 1) CkExit(); 
    else thisProxy[thisIndex + 1].printHello(); //tell the next element to print hello
  }
};

#include "arr.def.h"
