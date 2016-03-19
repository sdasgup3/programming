#include "stdio.h"
#include "MyModule.h" //based on module name

float targetDiff;
int gridWidth;
int gridHeight;
int tileWidth;
int tileHeight;


Main::Main(CkMigrateMessage * msg) {}
Main::Main(CkArgMsg * msg) 
{
  processCommandLine(msg);
  displayHeader();
  delete msg;
  // Create the grid tiles and set the reduction client
  grid = CProxy_Tile::ckNew(gridWidth, gridHeight);
  CkCallback cb(CkReductionTarget(Main, reductionCallback), thisProxy);
  grid.ckSetReductionClient(&cb);
  // Start the first step
  grid.startStep();
}


void Main::processCommandLine(const CkArgMsg * const msg)
{
  if(msg->argc < 5) {
    ckout << "exec <Error Tolerance> <Grid Size (a y)> <Tile Size (x y)>" << endl;
    CkExit();
  }

  targetDiff  = atof(msg->argv[1]);  
  gridWidth   = atoi(msg->argv[2]);  
  gridHeight  = atoi(msg->argv[3]);  
  tileWidth   = atoi(msg->argv[4]);  
  tileHeight  = atoi(msg->argv[5]);  
}

void Main::displayHeader()
{
  ckout << "  Error Tolerance: " << targetDiff << endl;
  ckout << "  Grid Size: [ " << gridWidth << " x " << gridHeight << " ]" << endl;
  ckout << "  Tile Size: [ " << tileWidth << " x " << tileHeight << " ]" << endl;
}


void Main::reductionCallback(float maxxDiff) 
{

}


void Tile::startStep() {
  const int thisX = thisIndex.x;
  const int thisY = thisIndex.y;
  // Send to the north (target tile receives from the south)
  float⇤ northGhost = tileData + NORTH OFFSET + TILE Y STEP;
  thisProxy(thisX, thisY > 0 ? thisY 1 : gridHeight 1).recvSouthGhost(northGhost, tileWidth);

  // Send to the south (target tile recieves from the north)
  float⇤ southData = tileData + SOUTH OFFSET TILE Y STEP;
  thisProxy(thisX, thisY < gridHeight 1 ? thisY + 1 : 0).recvNorthGhost(southData, tileWidth);

  // Send to the west (target tile recieves from the east)
  for (int i = 0; i < tileHeight; i++)
    scratchData[i] = tileData[WEST OFFSET + TILE X STEP + (TILE Y STEP ⇤ i)];
  thisProxy(thisX > 0 ? thisX 1 : gridWidth 1, thisY).recvEastGhost(scratchData, tileHeight);

  // Send to the east (target tile recieves from the west)
  for (int i = 0; i < tileHeight; i++)
    scratchData[i] = tileData[EAST OFFSET TILE X STEP + (TILE Y STEP ⇤ i)];
  thisProxy(thisX < gridWidth 1 ? thisX + 1 : 0, thisY).recvWestGhost(scratchData, tileHeight);
  countEvent();
}





#include "MyModule.def.h" //based on module name

