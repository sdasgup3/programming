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
#include "llvm/IR/Constants.h"
#include "llvm/ADT/SmallVector.h"
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
    bool promoteAllocasToRegs(Function &);

    SmallVector<AllocaInst*, 32>* performSROA(SmallVector<AllocaInst*, 32> *);

    // getAnalysisUsage - List passes required by this pass.  We also know it
    // will not alter the CFG, so say so.
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.setPreservesCFG();
    }

    private:
      bool isAllocaPromotable(AllocaInst*);
      void cleanUnusedAllocas(Function&);
      bool isAllocaExpandable(AllocaInst*);
      bool test_U1(GetElementPtrInst*);
      bool test_U2(ICmpInst*);
      bool checkFormat(GetElementPtrInst*);
      bool isSafeLoad_or_Store(Instruction*, Value*);
      bool test_U1_or_U2(Instruction*);
      bool test_U1(Instruction*);
      bool test_U2(Instruction*);
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
 * Function :   SROA::runOnFunction
 * Purpose  :   Entry point for the overall ScalarReplAggregates function pass.
********************************************************************/
bool SROA::runOnFunction(Function &F) {

  bool Changed = promoteAllocasToRegs(F);

  //Collect the initial set of allocas instructions
  SmallVector<AllocaInst*, 32> *Allocas;

  BasicBlock &BB = F.getEntryBlock();  
  for (BasicBlock::iterator I = BB.begin(), E = --BB.end(); I != E; ++I) {
    if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {       
      Allocas->push_back(AI);  
    }
  }

  while (!Allocas->empty()) {

    Allocas = performSROA(Allocas);

    if (NULL == Allocas) {
      break; 
    }

    promoteAllocasToRegs(F);
    Changed = true;
  }
  
  if(Changed) {
    //cleanUnusedAllocas(F);
  }
  
  return Changed;
}

/*******************************************************************
 *  Function :  SROA::promoteAllocasToRegs
 *  Purpose  :  Promote allocas to registers, which can enable more 
 *              scalar replacement.
********************************************************************/
bool SROA::promoteAllocasToRegs(Function &F)
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

/*******************************************************************************************
 *  Function: SROA::isAllocaPromotable
 *  Purpose : To check if the Alloca instructions, AI is promotable
 *            An object allocated using an alloca instruction is promotable to live in a 
 *            register if the alloca satisfies all these requirements:
 *
 *            (P1)  The alloca is a “first-class” type, which you can approximate conservatively with
 *                  isFPOrFPVectorTy() || isIntOrIntVectorTy() || isPtrOrPtrVectorTy.
 *            (P2)  The alloca is only used as the pointer argument of a load or store instruction (i.e., not the value being
 *                  stored), and the instruction satisfies !isVolatile().
 *
 ***************************************************************/ 
bool SROA::isAllocaPromotable(AllocaInst* AI)
{
  Type* AIType = AI->getAllocatedType(); 
  
  if(false == AIType->isFPOrFPVectorTy() && false == AIType->isIntOrIntVectorTy() && 
      false == AIType->isPtrOrPtrVectorTy()) {
    return false;
  }

  for (Value::use_iterator UI = AI->use_begin(), UE = AI->use_end(); 
        UI != UE; ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    
    if(isSafeLoad_or_Store(I, AI)) {
      continue;
    }
    return false;
  }

  return true;
}

bool SROA::isSafeLoad_or_Store(Instruction* I, Value* P)
{
  if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
    if (LI->isVolatile()) {
        return false;
    } 
  } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
    if (SI->getOperand(0) == P) {
        return false; 
    }
    if (SI->isVolatile()) {
        return false;
    }
  }
  return true;
}

SmallVector<AllocaInst*, 32>* SROA::performSROA(
    SmallVector<AllocaInst*, 32> *Allocas)
{
  SmallVector<AllocaInst*, 32> *newAllocas = new SmallVector<AllocaInst*, 32>();
  bool isExpanded = false;

  for(SmallVector<AllocaInst*, 32>::iterator AI = Allocas->begin(), 
      E = Allocas->end(); AI != E; ++AI)  {
    if(isAllocaExpandable(*AI)) {
      errs()<< "\n\nExpandable:: " << *AI << "\n\n";
      //expandAlloca(*AI, newAllocas);
      //isExpanded = true;
    }
  }

  if(true == isExpanded) {
    return newAllocas;
  } else {
    return NULL;
  }

}

bool SROA::isAllocaExpandable(AllocaInst *AI) 
{
  errs()<< "isAllocaExpandable Processing ... " << *AI << "\n\n";

  //Only need to consider alloca instructions that 
  //allocate an object of a structure type.
  if(!AI->getAllocatedType()->isStructTy())
    return false;

  //Are the Uses Safe
  return test_U1_or_U2(AI);
}

bool SROA::test_U1_or_U2(Instruction *AI)
{
  for (Value::use_iterator UI = AI->use_begin(), UE = AI->use_end(); 
        UI != UE; ++UI) {
    Instruction *I = cast<Instruction>(*UI);

    if(test_U1(I)) {
      continue;
    } 

    if(test_U2(I)) {
        continue;
    }

    return false;
  }
  return true;
}

//It is of the form: getelementptr ptr, 0, constant[, ... constant].
bool SROA::test_U1(Instruction* AI)
{
  GetElementPtrInst* G = dyn_cast<GetElementPtrInst>(AI);
  if(!G) {  
    return false;
  }

  errs() << "GEP: " << *G << "\n";  
  
  if(!checkFormat(G)) {
    return false;
  }

  for (Value::use_iterator UI = G->use_begin(), UE = G->use_end(); 
        UI != UE; ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    
    if(test_U1_or_U2(I)) {
      continue;
    }
    
    if(isSafeLoad_or_Store(I, G)) {
      continue;
    }
    
    return false;
  }
  return true;
}

bool SROA::checkFormat(GetElementPtrInst* G)
{
   // two indices
  unsigned NumIndices = G->getNumIndices();
  if(NumIndices != 2)
    return false;
  
  PointerType* PtrTy = dyn_cast<PointerType>(G->getPointerOperandType());
  if (!PtrTy) {
    return false;
  }
  // the first is zero
  if(ConstantInt *OP = dyn_cast<ConstantInt>(G->getOperand(1))) {
    if(!OP->isZero()) {
      return false;
    }
  }

  // the second onwards is constant
  for (unsigned i = 2; i <= NumIndices; ++i) {
    if(!isa<ConstantInt>(G->getOperand(i))) {
      return false;
    }
  }
  
  return true;
}

bool SROA::test_U2(Instruction* AI) 
{
  ICmpInst*  I = dyn_cast<ICmpInst>(AI);
  if(!I) {
    return false;
  }

  if(Constant *V = dyn_cast<Constant>(I->getOperand(0)))  {
    if(V->isNullValue())  { 
      return true;
    }
  }
  
  if(Constant *V = dyn_cast<Constant>(I->getOperand(1))) {
    if(V->isNullValue()) {
      return true;
    }
  }

  return false;
}

void SROA::cleanUnusedAllocas(Function &F) {
  BasicBlock &BB = F.getEntryBlock();
  for (BasicBlock::iterator I = BB.begin(), E = --BB.end(); I != E; ++I) {
    if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {
      if(AI->getNumUses() == 0) {
        AI->eraseFromParent();
      } 
    }
  }
}
