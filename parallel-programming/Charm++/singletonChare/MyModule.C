#include "stdio.h"
#include "MyModule.decl.h" //based on module name


/*Main Chare*/
class Main : public CBase_Main {
  private:
    int count;
  public:
  Main(CkArgMsg* msg) {
    ckout << "Hello World!" << endl;
    if(msg->argc > 1) {
      ckout << "and Hello " << msg->argv[1] <<endl;
    }
    double pi = 3.14;
    count = 10; 

    CProxy_Simple sm = CProxy_Simple::ckNew( pi, thisProxy);
    for(int i = 1 ; i <=10; i++) {
      sm.findArea(i, 0);
    }

    //CkExit(); // Makes the program hang
  };

  void printArea(int r, double area) {
    ckout << "Area of a circle of radius" << r << " is "<< area << endl;
    count --;
    if(0 == count ) {
      CkExit();
    }
  };
};

class Simple : public CBase_Simple {
  private:
    double y;
    CProxy_Main mainProxy;
  public:
    Simple(double pi, CProxy_Main master) {
      y = pi;
      mainProxy  = master;
      ckout << "Hello from a simple chare running on " << CkMyPe() << endl;
    };

    void findArea(int r, bool done) {
      mainProxy.printArea(r, y*r*r);
    };

};



#include "MyModule.def.h" //based on module name

