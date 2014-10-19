#include <stdlib.h>
#include <vector>
#include "pup_stl.h"
#include "Particle.h"
#include "ParticleExercise.decl.h"

#define ITERATION (100)
#define GRID_MAX (100)
#define GRID_MIN (0)
#define LB_INTERVAL 5
#define wrap_chare(a)  (((a)+cellDimension)%cellDimension)

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_Cell cellProxy;
/*readonly*/ int particlesPerCell;
/*readonly*/ int cellDimension;

using namespace std;
CkReduction::reducerType maxAndAvgType;

CkReductionMsg *maxAndAvg(int nMsg,CkReductionMsg **msgs)
{
  //maxAndAvg[1] will store the max
  //maxAndAvg[2] will store the total
  int maxNavg[3] = {0,-1, 0};

  int iteration;

  for (int i=0;i<nMsg;i++) {

    CkAssert(msgs[i]->getSize()== 3*sizeof(int));

    int *m   =(int *)msgs[i]->getData();
    int size =msgs[i]->getSize();


    //CkPrintf("Data: (%d %d %d) Size: %d\n", m[0],m[1], m[2], size);

    iteration = m[0];
    if(maxNavg[1] < m[1]) {
      maxNavg[1]  = m[1];
    }
    maxNavg[2]  +=  m[2];
  }
  maxNavg[0]  = iteration; 

  return CkReductionMsg::buildNew(3*sizeof(int),maxNavg);
}

/*initnode*/ void registerMaxAndAvg(void)
{
  maxAndAvgType = CkReduction::addReducer(maxAndAvg);
}

class Main: public CBase_Main {

  public:
    int doneCells;

    Main(CkArgMsg* m) {
      doneCells = 0;
      if(m->argc < 3) 
        CkAbort("USAGE: ./charmrun +p<number_of_processors> ./particle <number of particles per cell> <size of array>");

      mainProxy = thisProxy;
      particlesPerCell = atoi(m->argv[1]);
      cellDimension = atoi(m->argv[2]);

      if(cellDimension > 100 || cellDimension < 1) {
        CkAbort("The cellDimension must be in the range [1,100]\n");
      }

      delete m;

      //TODO: Create the grid and start the simulation by calling run()
      cellProxy = CProxy_Cell::ckNew(cellDimension,cellDimension);

      //Setting default call back
      //CkCallback *cb = new CkCallback(CkIndex_Main::Result(NULL),  mainProxy);
      //cellProxy.ckSetReductionClient(cb);

      cellProxy.run();
    }

    // TODO: Add entry methods which will be a target of the reduction for avg
    // and max counts and exiting when the iterations are done
    void done() {
      CkExit();
    }

    void Result(CkReductionMsg *msg) {
      int *data=(int *) msg->getData();
      printTotal(data[2],data[1],data[0]); //DSAND: iteratioon
      delete msg;
    }
    

    void printTotal(int total, int max, int iter){
        CkPrintf("ITER %d, MAX: %d, TOTAL: %d\n", iter, max, total);
    }
};

// This class represent the cells of the simulation.
/// Each cell contains a vector of particle.
// On each time step, the cell perturbs the particles and moves them to neighboring cells as necessary.
class Cell: public CBase_Cell {
  Cell_SDAG_CODE

  public:
    int iteration;
    vector<Particle> particles;
    //TODO: more variables might be needed
    double xMin, xMax, yMin, yMax;
    vector<Particle> N, S, E, W, NE, NW, SE, SW, MINE;
    int count_neighbours;

    Cell() {
      __sdag_init();
      iteration = 0;
      initializeBoundary(); 
      populateCell(particlesPerCell); //creates random particles within the cell
      usesAtSync = CmiTrue;
    }

    Cell(CkMigrateMessage* m) {}

    void pup(PUP::er &p){
      CBase_Cell::pup(p);
      __sdag_pup(p);
      p|iteration;
      p|particles;
      p|xMin;
      p|xMax;
      p|yMin;
      p|yMax;
      p|N;
      p|S;
      p|E;
      p|W;
      p|NE;
      p|NW;
      p|SE;
      p|SW;
      p|MINE;
      p|count_neighbours;
      //TODO: if you added more variable, decide if they need to go into the pup method
    }

    void updateParticles() { //TO ASK: order of TODOs

      //Clear the buffers for the next iteration
      N.clear(); NE.clear(); E.clear(); SE.clear();
      S.clear(); SW.clear(); W.clear(); NW.clear(); MINE.clear();
      
      //TODO:use perturb function for the location of new particles
      //TODO:move the particles

      std::vector<Particle>::iterator itr = particles.begin();
      std::vector<Particle>::iterator end = particles.end();
      while (itr != end) {

        double x = (*itr).x;
        double y = (*itr).y;

        perturb(*itr);

        prepareForMovement(*itr);
        itr++;
      }
      particles = MINE;
    }
    //you can add more methods if you want or need to

  private:

    //change the location of the particle within the range of 8 neighbours
    //the location of the particles might exceed the bounds of the chare array
    //as a result of this functions, so you need to handle that case when deciding 
    //which particle to go which neighbour chare
    //e.g. the right neighbour of chare indexed[k-1,0] is chare [0,0]
    void perturb(Particle &particle) { //TO ASK change of proto

      //drand48             : creates a random number between [0-1)	
      //(drand48-drand48()) : creates a random number between (-1,+1)	
      //deltax or deltay    : creates a random number between (-100/k,+100/k)	
      double deltax = (drand48()-drand48())*(100/cellDimension);
      double deltay = (drand48()-drand48())*(100/cellDimension);

      double prev_x = particle.x;
      double prev_y = particle.y;

      particle.x += deltax;
      particle.y += deltay;

      double bound = ((double)GRID_MAX)/cellDimension ;
      CkAssert(particle.x >= (xMin - bound) && particle.x < (xMax + bound)
          && particle.y >= (yMin - bound) &&  particle.y < (yMax + bound));
      /*
      if(particle.x >= (xMin - bound) && particle.x < (xMax + bound)
          && particle.y >= (yMin - bound) &&  particle.y < (yMax + bound)) {
      }else {
        CkPrintf("Chare : [%d %d] Bounds : (%f %f %f %f) [%f %f] --> [%f %f] with delta (%f %f) %f %f\n\n",
            thisIndex.x, thisIndex.y, xMin, xMax, yMin, yMax, 
            prev_x, prev_y, particle.x, particle.y,  
            deltax, deltay);
        CkAbort("Perturn is Wrong!!\n\n");
      }
      */
    }

    void prepareForMovement(Particle &p) {
      
      double x = p.x;
      double y = p.y;
      
      if(x >= GRID_MAX) {
        p.x  = x - GRID_MAX;
      }
      if(x < GRID_MIN) {
        p.x  = x + GRID_MAX;
      }
      if(y >= GRID_MAX ) {
        p.y  = y - GRID_MAX;
      }
      if(y < GRID_MIN) {
        p.y  = y + GRID_MAX;
      }


      if(x >= xMax && y >= yMin && y < yMax) {
        //Particle moved to East: (thisIndex.x, thisIndex.y) to (thisIndex.x + 1, thisIndex,y)
          E.push_back(p);
      } else if(x >= xMax && y >= yMax) {
      //Particle moved to SouthEast: (thisIndex.x, thisIndex.y) to (thisIndex.x + 1, thisIndex,y + 1)
          SE.push_back(p);
      } else if(x >= xMin && x < xMax && y >= yMax) {
      //Particle moved to South: (thisIndex.x, thisIndex.y) to (thisIndex.x, thisIndex,y + 1)
          S.push_back(p);
      } else if(x < xMin && y >= yMax) {
      //Particle moved to SouthWest: (thisIndex.x, thisIndex.y) to (thisIndex.x - 1, thisIndex,y + 1)
          SW.push_back(p);
      } else if(x < xMin && y >= yMin && y < yMax) {
      //Particle moved to West: (thisIndex.x, thisIndex.y) to (thisIndex.x - 1, thisIndex,y)
          W.push_back(p);
      } else if(x < xMin && y < yMin) {
      //Particle moved to NorthWest: (thisIndex.x, thisIndex.y) to (thisIndex.x - 1, thisIndex,y - 1)
          NW.push_back(p);
      } else if(x >= xMin && x < xMax && y < yMin) {
      //Particle moved to North: (thisIndex.x, thisIndex.y) to (thisIndex.x , thisIndex,y - 1 )
          N.push_back(p);
      } else if(x >= xMax && y < yMin) {
      //Particle moved to NorthEast: (thisIndex.x, thisIndex.y) to (thisIndex.x + 1, thisIndex.y -1)
          NE.push_back(p);
      } else {
      //Still positioned in the same cell
          MINE.push_back(p);
      }
    }


    /* The cell boundaries are considered as integers
     * For example: if k = 16, the the x - coordinate space
     * will get divided as [0-6.25), [6.25-12.50) ... [93.75,100)         //TO ASK
     */
    void initializeBoundary() {
      double bound = ((double)GRID_MAX)/cellDimension ;
      double temp;

      xMin = (thisIndex.x * bound);
      temp = (xMin + bound);
      xMax = temp > GRID_MAX ? GRID_MAX : temp;
      xMax = temp > GRID_MAX ? GRID_MAX : temp;

      yMin = (thisIndex.y * bound);
      temp = (yMin + bound);
      yMax = temp > GRID_MAX ? GRID_MAX : temp;

      //CkPrintf("[%d %d](%f %f %f %f): \n",thisIndex.x, thisIndex.y,  xMin, xMax, yMin, yMax);
      CkAssert(xMin < xMax || yMin < yMax);
    }

    /* For a cell with boundary defined as (xMin,xMax) and (yMin,yMax),
     * The following fucntion generate particles with coordinates
     * range as x = [xMin,xMax), y = [yMin,yMax)
     */
    void populateCell(int initialElements) {
      initialElements = initialElements / 2 + (thisIndex.x *initialElements) / cellDimension;
      srand(time(NULL) + thisIndex.x + thisIndex.y); //TO ASK 
      for(int i = 0 ; i < initialElements;  i++) {
        double f_x = (double)rand() / RAND_MAX;
        double f_y = (double)rand() / RAND_MAX;
        double rand_x  = f_x * (xMax - xMin - 1 ) + xMin;
        double rand_y  = f_y * (yMax - yMin - 1 ) + yMin;
        Particle p(rand_x, rand_y);
        particles.push_back(p);
      }
    }

};

#include "ParticleExercise.def.h"
