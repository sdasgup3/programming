//===- ScalarReplAggregates.cpp - Scalar Replacement of Aggregates --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This transformation implements the well known scalar replacement of
// aggregates transformation.  This xform breaks up alloca instructions of
// structure type into individual alloca instructions for
// each member (if possible).  Then, if possible, it transforms the individual
// alloca instructions into nice clean scalar SSA form.
//
// This combines an SRoA algorithm with Mem2Reg because they
// often interact, especially for C++ programs.  As such, this code
// iterates between SRoA and Mem2Reg until we run out of things to promote.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "scalarrepl"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/Statistic.h"
using namespace llvm;

//STATISTIC(NumReplaced,  "Number of aggregate allocas broken up");
STATISTIC(NumPromoted,  "Number of scalar allocas promoted to register");

namespace {
  struct SROA : public FunctionPass {
    static char ID; // Pass identification
    SROA() : FunctionPass(ID) { }

    // Entry point for the overall scalar-replacement pass
    bool runOnFunction(Function &F);

    //The following mem2reg step promotes some scalar memory locations
    bool PromoteAllocasToRegs(Function &);

    bool isAllocaPromotable(const AllocaInst*);


    // getAnalysisUsage - List passes required by this pass.  We also know it
    // will not alter the CFG, so say so.
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesCFG();
      AU.addRequired<DominatorTreeWrapperPass>();
    }


  };
}

char SROA::ID = 0;
static RegisterPass<SROA> X("scalarrepl-sdasgup3",
                            "Scalar Replacement of Aggregates (by sdasgup3)",
                            false /* does not modify the CFG */,
                            false /* transformation, not just analysis */);


/*************************************************************
 * Function : createMyScalarReplAggregatesPass
 * Purpose  : Public interface to create the ScalarReplAggregates pass.
**************************************************************/
FunctionPass *createMyScalarReplAggregatesPass() { 
  return new SROA(); 
}


/*******************************************************************
 * Function :   runOnFunction:
 * Purpose  :   Entry point for the overall ScalarReplAggregates function pass.
********************************************************************/
bool SROA::runOnFunction(Function &F) {

  bool Changed = PromoteAllocasToRegs(F);

  return Changed;
}

bool SROA::PromoteAllocasToRegs(Function &F)
{
  bool Changed  = false;
  std::vector<AllocaInst*> Allocas;

  BasicBlock &BB = F.getEntryBlock();  
  DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  
  while(1) {
    Allocas.clear();

    for (BasicBlock::iterator I = BB.begin(), E = --BB.end(); I != E; ++I) {
      if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {       
        if (isAllocaPromotable(AI)) {
          Allocas.push_back(AI);  
        }
      }
    }

    if (Allocas.empty()) break;

    PromoteMemToReg(Allocas, DT);
    NumPromoted += Allocas.size();
    Changed = true;
  }
  return Changed;
}

bool SROA::isAllocaPromotable(const AllocaInst* AI)
{
  Type* AIType = AI->getType(); 
  
  if(false == AIType->isFPOrFPVectorTy() && false == AIType->isIntOrIntVectorTy() && 
      false == AIType->isPtrOrPtrVectorTy()) {
    return false;
  }
  
  return true;
}
