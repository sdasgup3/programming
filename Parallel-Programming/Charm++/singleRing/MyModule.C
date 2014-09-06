#include "stdio.h"
#include "MyModule.decl.h" //based on module name

class Main: public CBase_Main {
  public:
  Main(CkArgMsg* msg) {
    int ringSize  = atoi(msg->argv[1]);
    int tripCount = atoi(msg->argv[2]);
    delete msg;

    CkPrintf("\"Array Ring (Single)\" Program\n");
    CkPrintf(" ringSize = %d, tripCount = %d, #PEs() = %d\n", ringSize, tripCount, CkNumPes());

    // Create the chare array and start at a random element
    CProxy_Ring ring = CProxy_Ring::ckNew(thisProxy, ringSize, ringSize);
    srand(time(NULL)); // Initialize random number generator
    //ring(rand() % ringSize).do(ringSize, tripCount, -1, -1);
    ring(2).doSomething(ringSize, tripCount, -1, -1);
  }

  void ringFinished() {
    CkExit();
  }
};


class Ring: public CBase_Ring {
  private:
    CProxy_Main mainProxy; // Proxy object for the main chare
    int ringSize; // Number of elements in the ring
  public:
    Ring(CkMigrateMessage *msg) {}
    Ring(CProxy_Main mp, int rs) {
      mainProxy = mp;
      ringSize = rs;
    }

    inline int nextI() { 
      return ((thisIndex + 1) % ringSize); 
    }

    void doSomething(int elementsLeft, int tripsLeft, int fromIndex, int fromPE) {
      // Do something (display some text for the user)
      printf("Ring[%d](%d): tripsLeft = %d, from [%d](%d)\n", thisIndex, CkMyPe(), tripsLeft, fromIndex, fromPE);

      // Send message to continue traversals or notify main
      if (elementsLeft > 1) { // elements left in traversal
        thisProxy(nextI()).doSomething(elementsLeft - 1, tripsLeft, thisIndex, CkMyPe());
      } else if (tripsLeft > 1) { // start next traversal
        thisProxy(nextI()).doSomething(ringSize, tripsLeft - 1, thisIndex, CkMyPe());
      } else { // otherwise, all traversals finished
        mainProxy.ringFinished();
      }
   }
};

#include "MyModule.def.h"
