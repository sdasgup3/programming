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

    CProxy_Simple sm = CProxy_Simple::ckNew( pi, 10);

  };

};

class Simple : public CBase_Simple {
  private:
    double y;
    int cnt;  
  public:
    Simple(double pi, int count) {
      y = pi;
      cnt = count ;
      ckout << "Hello from a simple chare running on " << CkMyPe() << endl;
      findArea();
    };

    void findArea() {
      for(int i = 1 ; i <=10; i++) {
        ckout << "Area of a circle of radius " << i << " is "<< y*i*i << endl;
      }
      CkExit();
    };

};



#include "MyModule.def.h" //based on module name

