#include "MyModule.decl.h" 

class Main : public CBase_Main {
  private:
    CProxy_Tile grid;
    void processCommandLine(const CkArgMsg * const msg);
    void displayHeader();

  public:
    Main(CkMigrateMessage * msg);
    Main(CkArgMsg* msg);
    void reductionCallback(float );
};

class Tile: public CBase_Tile {
  public:
    Tile(CkMigrateMessage * msg){}
    Tile() {}
};
