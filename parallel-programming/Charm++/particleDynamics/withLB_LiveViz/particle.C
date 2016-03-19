#include "liveViz.h"
#include <stdlib.h>
#include <vector>
#include "pup_stl.h"
#include "Particle.h"
#include "ParticleExercise.decl.h"
#include <algorithm>

#define ITERATION (100)
#define GRID_MAX (100)
#define GRID_MIN (0)
#define wrap_chare(a)  (((a)+cellDimension)%cellDimension)
typedef enum { GREEN, BLUE, RED, BLUE_GREEN} colorType;

CProxy_Main mainProxy;
CProxy_Cell cellProxy;
int particlesPerCell;
int cellDimension;
double centreSquare_xMin ;
double centreSquare_xMax; 
double centreSquare_yMin ;
double centreSquare_yMax  ;



using namespace std;
CkReduction::reducerType maxAndAvgType;

CkReductionMsg *maxAndAvg(int nMsg,CkReductionMsg **msgs)
{
  int maxNavg[3] = {0,-1, 0};

  int iteration;

  for (int i=0;i<nMsg;i++) {

    CkAssert(msgs[i]->getSize()== 3*sizeof(int));

    int *m   =(int *)msgs[i]->getData();
    int size =msgs[i]->getSize();

    iteration = m[0];
    if(maxNavg[1] < m[1]) {
      maxNavg[1]  = m[1];
    }
    maxNavg[2]  +=  m[2];
  }
  maxNavg[0]  = iteration; 

  return CkReductionMsg::buildNew(3*sizeof(int),maxNavg);
}

void registerMaxAndAvg(void)
{
  maxAndAvgType = CkReduction::addReducer(maxAndAvg);
}

class Main: public CBase_Main {

  public:
    int doneCells;
    int count;

    Main(CkArgMsg* m) {
      doneCells = 0;
      count = 0;
      if(m->argc < 3) 
        CkAbort("USAGE: ./charmrun +p<number_of_processors> ./particle <number of particles per cell> <size of array>");

      mainProxy = thisProxy;
      particlesPerCell = atoi(m->argv[1]);
      cellDimension = atoi(m->argv[2]);

      if(cellDimension > 100 || cellDimension < 1) {
        CkAbort("The cellDimension must be in the range [1,100]\n");
      }

      delete m;

      centreSquare_xMin  =  37.50 ;   // 50 -  25/2
      centreSquare_xMax  =  62.50 ;   // 50 +  25/2
      centreSquare_yMin  =  37.50 ;
      centreSquare_yMax  =  62.50 ;

      CkArrayOptions opts(cellDimension, cellDimension);
      cellProxy = CProxy_Cell::ckNew(opts);

      // setup liveviz
      CkCallback c(CkIndex_Cell::requestNextFrame(0),cellProxy);
      liveVizConfig cfg(liveVizConfig::pix_color,true);
      liveVizInit(cfg,cellProxy,c, opts);

      cellProxy.run();
    }

    void done() {
      CkExit();
    }

    void Result(CkReductionMsg *msg) {
      int *data=(int *) msg->getData();
      printTotal(data[2],data[1],data[0]); 
      delete msg;
    }
    

    void printTotal(int total, int max, int iter){
        CkPrintf("ITER %d, MAX: %d, TOTAL: %d\n", iter, max, total);
    }
};

class Cell: public CBase_Cell {
  Cell_SDAG_CODE

  public:
    int iteration;
    vector<Particle> particles;
    double xMin, xMax, yMin, yMax;
    vector<Particle> N, S, E, W, NE, NW, SE, SW, MINE;
    int count_neighbours;

    Cell() {
      __sdag_init();
      iteration = 0;
      initializeBoundary(); 
      populateCell(particlesPerCell); 
      usesAtSync = CmiTrue;
    }

    Cell(CkMigrateMessage* m) {}

    void pup(PUP::er &p){
      CBase_Cell::pup(p);
      __sdag_pup(p);
      p|iteration;
      p|particles;
      p|xMin; p|xMax; p|yMin; p|yMax;
      p|N; p|S; p|E; p|W; p|NE; p|NW; p|SE; p|SW; p|MINE;
      p|count_neighbours;
    }

    void updateParticles() { 

      N.clear(); NE.clear(); E.clear(); SE.clear();
      S.clear(); SW.clear(); W.clear(); NW.clear(); MINE.clear();
      
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

    void color_pixel(unsigned char*buf,int width, int height, int xpos,int ypos,
                             unsigned char R,unsigned char G,unsigned char B) {
      if(xpos>=0 && xpos<width && ypos>=0 && ypos<height) {
        if(R)
          buf[3*(ypos*width+xpos)+0] = R; 
        if(G)
          buf[3*(ypos*width+xpos)+1] = G; 
        if(B)
          buf[3*(ypos*width+xpos)+2] = B; 
      }
    }

    void requestNextFrame(liveVizRequestMsg *m){
      int wdes = m->req.wid;
      int hdes = m->req.ht;

      int w = wdes/cellDimension;
      int h = hdes/cellDimension;
      int sx  = thisIndex.x * w;
      int sy  = thisIndex.y * h ;
      double bound = ((double)GRID_MAX)/cellDimension ;

      unsigned char *intensity= new unsigned char[3*w*h];
      CkPrintf("requestNextFrame\n");
      for(int i = 0 ; i < 3*w*h ; i ++) {
        intensity[i] = 0;
      }

      for (int i=0; i < particles.size(); i++ ) {
        int xpos = (int)(((particles[i].x - xMin)/bound)*w);
        int ypos = (int)(((particles[i].y - yMin)/bound)*h);

        //CkAssert(xpos>=0 && xpos<w && ypos>=0 && ypos<h);
        color_pixel(intensity,w,h,xpos,ypos,particles[i].R,particles[i].G,particles[i].B); 
      }



      for(int i=0;i<h;++i){
        intensity[3*(i*w+w-1)+0] = 0;     // RED component
        intensity[3*(i*w+w-1)+1] = 0;   // GREEN component
        intensity[3*(i*w+w-1)+2] = 0;     // BLUE component
      }
      for(int i=0;i<w;++i){
        intensity[3*((h-1)*w+i)+0] = 0;   // RED component
        intensity[3*((h-1)*w+i)+1] = 0; // GREEN component
        intensity[3*((h-1)*w+i)+2] = 0;   // BLUE component
      }

      /**/
      
      liveVizDeposit(m, sx,sy, w,h, intensity, this);
      delete[] intensity;
    }

  private:

    void perturb(Particle &particle) { 

      //  drand48             : creates a random number between [0-1)	
      //  (drand48-drand48()) : creates a random number between (-1,+1)	
      //  deltax or deltay    : creates a random number between (-100/k,+100/k)	

      double deltax = (drand48()-drand48())*(100/cellDimension);
      double deltay = (drand48()-drand48())*(100/cellDimension);

      if(particle.G ==  255) {
        deltax  = deltax / 4;
        deltay  = deltay / 4;
      } else if(particle.B  ==  255) {
        deltax  = deltax / 2;
        deltay  = deltay / 2;
      }

      particle.x += deltax;
      particle.y += deltay;

      double bound = ((double)GRID_MAX)/cellDimension ;
      CkAssert(particle.x >= (xMin - bound) && particle.x < (xMax + bound)
          && particle.y >= (yMin - bound) &&  particle.y < (yMax + bound));
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


    /* 
     * For example: if k = 16, the the x - coordinate space
     * will get divided as [0-6.25), [6.25-12.50) ... [93.75,100)      
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
      srand(time(NULL) + thisIndex.x + thisIndex.y); 

      if(thisIndex.x > thisIndex.y) {               /* Upper triangle: populate GREEN */
        populateCellHelper(initialElements, GREEN, 0 );
      } else if(thisIndex.x < thisIndex.y) {        /* Lower triangle: populate BLUE */       
        populateCellHelper(initialElements, BLUE, 0 );
      } else if(thisIndex.x == thisIndex.y) {       /* Diagonal: populate BLUE & GREEN */
        populateCellHelper(initialElements, BLUE, 1);
        populateCellHelper(initialElements, GREEN, 1 );
      }


      /* to find if the chare (thisIndex.x, thisIndex.y) overlaps with the center red patch*/
      if((centreSquare_xMax > xMin && xMax > centreSquare_xMin) &&
            (centreSquare_yMax > yMin && yMax > centreSquare_yMin)) {
        populateCellHelper(2*initialElements, RED, 1);
      }
        
    }

    void populateCellHelper(int initialElements, colorType color, int mayOverlap) {

      double new_xMin, new_xMax, new_yMin, new_yMax;
      if(RED == color) {
        /* Overlaped x-line segment: (overlap_xMin,0)  --- (overlap_xMax, 0) */
        /* Overlaped y-line segment: (0, overlap_yMin)  --- (0, overlap_yMax) */
        double overlap_xMin = max(centreSquare_xMin, xMin);
        double overlap_xMax = min(centreSquare_xMax, xMax);
        double overlap_yMin = max(centreSquare_yMin, yMin);
        double overlap_yMax = min(centreSquare_yMax, yMax);

        /* To find the portion of center red patch belonging to current chare*/
        new_xMin  = overlap_xMin;
        new_xMax  = overlap_xMax;
        new_yMin  = overlap_yMin;
        new_yMax  = overlap_yMax;

      } else {
        new_xMin  = xMin; new_xMax = xMax;
        new_yMin  = yMin; new_yMax = yMax;
      }


      for(int i = 0 ; i < initialElements;  i++) {
        double f_x = (double)rand() / RAND_MAX;
        double f_y = (double)rand() / RAND_MAX;
        double rand_x  = f_x * (new_xMax - new_xMin - 1 ) + new_xMin;
        double rand_y  = f_y * (new_yMax - new_yMin - 1 ) + new_yMin;

        if(GREEN == color) {
          if(0 == mayOverlap)
            CkAssert(rand_x >= rand_y);
          Particle p(rand_x, rand_y, 0, 255, 0);
          particles.push_back(p);

        } else if(BLUE == color) {
          if(0 == mayOverlap)
            CkAssert(rand_x <= rand_y);
          Particle p(rand_x, rand_y, 0, 0, 255);
          particles.push_back(p);

        } else if(RED == color) {
          CkAssert(rand_x >= centreSquare_xMin && rand_x < centreSquare_xMax && rand_y >= centreSquare_yMin && rand_y < centreSquare_yMax);
          Particle p(rand_x, rand_y, 255,0,0);
          particles.push_back(p);
        }
      }
    }

};

#include "ParticleExercise.def.h"
