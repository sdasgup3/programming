#include <stdio.h>
#include <math.h>
#include "ParPrefix.decl.h" //based on module name

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_Prefix prefixArray;
/*readonly*/ int numElements;

class Main : public CBase_Main {
  public:
  Main(CkArgMsg* msg) {
    mainProxy= thisProxy;
    if (msg->argc > 1) 
      numElements = atoi(msg->argv[1]);
    numElements = 8; // Default numElements to 8
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
    int* valueBuf, *flagBuf, value, stage, numStages;
  public:
  Prefix() {
    stage = 0;
    numStages = log2(numElements);
    valueBuf  = (int *) malloc(sizeof(int)*numStages);
    flagBuf   = (int *) malloc(sizeof(int)*numStages);
    for(int i = 0 ; i < numStages; i ++ ) {
      valueBuf[i] = 0;
      flagBuf[i] = 0;
    }
    //srand(time(NULL)+ thisIndex);
    srand(time(NULL));
    value = rand() % 10 + 1;
    step(value);
  }
  Prefix (CkMigrateMessage*) {};
  void step(int value) {

    if(stage >= numStages) {
      CkPrintf("\nPrefix[%d].value = %d\n", thisIndex, value);
      contribute(CkCallback(CkReductionTarget(Main, done), mainProxy));
    } else {
      int sendIndex = thisIndex + (1 << stage);
      if(sendIndex < numElements) {
        thisProxy[sendIndex].passValue(stage, value);
      }
      //if you no longer receive, but need to continue sending
      if(thisIndex - (1<<stage) < 0) {
        stage ++;
        step(value);
      }
    } 
  }

  void passValue(int incoming_stage, int incoming_value) {
    valueBuf[incoming_stage]  = incoming_value;
    flagBuf[incoming_stage]   = 1;

    for(int i = stage; i < numStages; i++) {
      if(flagBuf[i] == 0) break;

      value += valueBuf[i];
      stage ++;
      step(value);
    }
  }
};

#include "ParPrefix.def.h" //based on module name
