#include <stdio.h>
#include <math.h>
#include "MyModule.decl.h" //based on module name

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_Prefix prefixArray;
/*readonly*/ int numElements;

class Main : public CBase_Main {
  public:
  Main(CkArgMsg* msg) {
    mainProxy= thisProxy;
    numElements = 8; // Default numElements to 8
    if (msg->argc > 1) 
      numElements = atoi(msg->argv[1]);
    delete msg;
              
    prefixArray = CProxy_Prefix::ckNew(numElements);
  }
  Main(CkMigrateMessage* msg) {}
  void done() { 
    CkExit(); 
  }
};
class Prefix : public CBase_Prefix {
  private:
    int value;
    int distance;
  public:
  Prefix() {
    distance = 1;
    srand(time(NULL));
    value = rand() % 10;
    step();
  }
  Prefix (CkMigrateMessage*) {};
  void step() {
    if(distance < numElements) {
      if(thisIndex+distance<numElements) {
        thisProxy[thisIndex+distance].passValue(value);
      }
      //if you no longer receive, but need to continue sending
      if(thisIndex - distance < 0) {
        distance = distance*2;
        step();
      }
    } else {
      CkPrintf("\nPrefix[%d].value = %d\n", thisIndex, value);
      contribute(CkCallback(CkReductionTarget(Main, done), mainProxy));
    }
  }

  void passValue(int incoming_value) {
    value += incoming_value;
    distance = distance*2;
    step();
  }
};

#include "MyModule.def.h" //based on module name
