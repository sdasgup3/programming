#include "stdio.h"
#include "MyModule.decl.h" //based on module name

CProxy_Master mainProxy; //readonly

/*Main Chare*/
class Master : public CBase_Master {
  private:
    int count, totalInsideCircle, totalNumTrials;
    double startTime, endTime;
  public:
  Master(CkArgMsg* msg) {

    if(msg->argc < 3) {
      ckout << "Usage: exec unmTrial numChares\n";
      CkExit();
    }

    int numTrials = atoi(msg->argv[1]); 
    int numChares = atoi(msg->argv[2]); 

    if (numTrials % numChares) {
      ckout << "Need numTrials to be a divisible by numChares.. Sorry" << endl;
      CkExit();
    }

    startTime = CkWallTimer() ;
    for(int i = 1 ; i <=numChares; i++) {
      CProxy_Worker::ckNew(numTrials);
    }
    
    count = numChares; 
    mainProxy = thisProxy;
    totalInsideCircle = totalNumTrials = 0;

  };

  void addContribution(int numIn, int numTrials) {
    totalInsideCircle += numIn;
    totalNumTrials += numTrials;
    count--;
    if (count == 0) {
      double myPi = 4.0* ((double) (totalInsideCircle)) / ((double) (totalNumTrials));
      endTime = CkWallTimer() ;
      ckout << "Approximated value of pi is:" << myPi << endl;
      ckout << "Approximated time elapsed :" << (endTime - startTime) << endl;
      CkExit();
    }
  }
};

class Worker : public CBase_Worker {
  public:
    Worker(int numTrials ) {
      int inTheCircle = 0;
      double x, y;
      ckout << "Hello from a simple chare running on " << CkMyPe() << endl;
      for (int i = 0; i < numTrials; i++) {
        x = drand48();
        y = drand48();
        if ((x * x + y * y) < 1.0) inTheCircle++;
      }
      mainProxy.addContribution(inTheCircle, numTrials);
    };
};



#include "MyModule.def.h" //based on module name

