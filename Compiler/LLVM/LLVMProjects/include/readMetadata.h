#include"llvm/Pass.h"
#include"llvm/IR/Module.h"

using namespace llvm;
using namespace std;


class readMetadata : public ModulePass {
  private:
  public:
    static char ID;
    readMetadata():ModulePass(ID) {};
    bool runOnModule(Module &M);
    void getAnalysisUsage(AnalysisUsage &AU) const;
};
