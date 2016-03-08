#include <cstdio>
#include <cstdlib>
#include <iomanip>

/* LLVM Header File
#include "llvm/Support/DataTypes.h"
*/
#include "llvm/Support/raw_ostream.h"
#include "llvm/DebugInfo.h"

/* Header file global to this project */
#include "readMetadata.h"


char  readMetadata::ID = 0;
bool readMetadata::runOnModule(Module &M) {

  Module::const_iterator i = M.begin();
  Module::const_iterator e = M.end();

  for(;i != e; i++) {
    const Function &F = *i;
    string name = F.getName();
    errs() << "Function Processing : "<< name << "\n";

    for(const BasicBlock &B : F) {
      for(const Instruction &I : B) {

        unsigned Line, Col;
        if (I.hasMetadata()) { 
          const DebugLoc &Loc = I.getDebugLoc();
          Line = Loc.getLine();
          Col = Loc.getCol();
          errs() << "\t"<< I << " : " << "\t\t" << Line << " " << Col << "\n";
        } else {
          errs() << "\t"<< I << " : " << -1 << " " << -1 << "\n";
        }
      }
    }
  }

  return false;
}

void readMetadata::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

static RegisterPass<readMetadata> RM("readmeta", "Reading the Debug Metadata information", true, true);
