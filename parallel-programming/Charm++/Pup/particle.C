#include <stdlib.h>
#include <vector>
#include "pup_stl.h"
#include "Particle.h"
#include "ParticleExercise.decl.h"

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_Cell cellProxy;
/*readonly*/ int cellDimension;
int particlesPerCell;

using namespace std;

class Main: public CBase_Main {

  public:

    Main(CkArgMsg* m) {

      mainProxy = thisProxy;
      particlesPerCell = atoi(m->argv[1]);
      cellDimension = 2;

      delete m;

      cellProxy = CProxy_Cell::ckNew(cellDimension);
      cellProxy.run();

      CkCallback cb = CkCallback(CkReductionTarget(Main, done), thisProxy);
      CkStartQD(cb);
    }

    void done() {
      CkExit();
    }

};

class Cell: public CBase_Cell {
  Cell_SDAG_CODE

  public:
    vector<Particle> particles;
    Cell() {
      __sdag_init();
      populateCell(particlesPerCell); //creates random particles within the cell
    }

    Cell(CkMigrateMessage* m) {}

  private:

    void populateCell(int initialElements) {
      srand(thisIndex + initialElements);
      CkPrintf("[%d] generating\n", thisIndex);

      double f_x = (double)rand()/RAND_MAX; 
      double f_y = (double)rand()/RAND_MAX;
      double rand_x  = f_x * 100;
      double rand_y  = f_y * 100;

      int size = (rand_x) + rand_y;
      int *arr = new int[size];
      for(int i = 0 ; i < size ; i++) {
        arr[i] = thisIndex + i;
      }

      Particle p(rand_x, rand_y, arr, size);
      particles.push_back(p);

      CkPrintf("================== done\n\n\n");
    }

};

#include "ParticleExercise.def.h"
