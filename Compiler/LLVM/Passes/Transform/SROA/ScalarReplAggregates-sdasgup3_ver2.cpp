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
//#define MYDEBUG 
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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

STATISTIC(NumReplaced,  "Number of aggregate allocas broken up");
STATISTIC(NumPromoted,  "Number of scalar allocas promoted to register");

namespace {
  struct SROA : public FunctionPass {
    static char ID; // Pass identification
    SROA() : FunctionPass(ID) { }

    bool runOnFunction(Function &F);
    bool promoteAllocasToRegs(Function &);
    SmallVector<AllocaInst*, 32>* performScalarExpansion(SmallVector<AllocaInst*, 32> *, Function&);

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
      bool checkFormat(GetElementPtrInst*);
      bool test_U1_or_U2(Instruction*);
      bool test_U1(Instruction*);
      bool test_U2(Instruction*);
      void expandAlloca(AllocaInst *, SmallVector<AllocaInst*, 32>*, Function& );
      bool replaceUses(Instruction* AI, unsigned offset, Value* newAlloca);
      //bool replaceUses(Instruction* , unsigned ,Instruction* );
      bool isSafeStore(StoreInst*, Value*  );
      bool isSafeLoad(LoadInst*);
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
  SmallVector<AllocaInst*, 32> *Allocas =  
                  new SmallVector<AllocaInst*, 32>();
  BasicBlock &BB = F.getEntryBlock();  
  for (BasicBlock::iterator I = BB.begin(), E = --BB.end(); I != E; ++I) {
    if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {       
      Allocas->push_back(AI);  
    }
  }
#ifdef MYDEBUG
  if(Allocas->empty()) {
  errs() << "No Allocas left...Next function \n\n\n";
  }
#endif  

  while (!Allocas->empty()) {

    Allocas = performScalarExpansion(Allocas, F);

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
  
#ifdef MYDEBUG
  errs() << "Promoted Alloca\n";
#endif  
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
#ifdef MYDEBUG
          errs() << "\t=====Iter============================" << "\n";
#endif  
  }
#ifdef MYDEBUG
  errs() << "\t=====mem2reg Ends============================" << "\n\n";
#endif  
  return Changed;
}

/*************************************************************************
 *  Function: SROA::isAllocaPromotable
 *  Purpose : To check if the Alloca instructions, AI is promotable
 *            An object allocated using an alloca instruction is 
 *            promotable to live in a register if the alloca satisfies 
 *            all these requirements:
 *
 *            (P1)  The alloca is a “first-class” type, which you can 
 *            approximate conservatively with
 *              isFPOrFPVectorTy() || isIntOrIntVectorTy() || isPtrOrPtrVectorTy.
 *            (P2)  The alloca is only used as the pointer argument of 
 *            a load or store instruction (i.e., not the value being stored), 
 *            and the instruction satisfies !isVolatile().
 *
 ***********************************************************************/ 
bool SROA::isAllocaPromotable(AllocaInst* AI)
{
#ifdef MYDEBUG
  errs() << "isAllocaPromotable : Processing" << *AI << "\n";
#endif  
  Type* AIType = AI->getAllocatedType(); 
  
  if(false == AIType->isFPOrFPVectorTy() && false == AIType->isIntOrIntVectorTy() && 
      false == AIType->isPtrOrPtrVectorTy()) {
#ifdef MYDEBUG
  errs() << "isAllocaPromotable : NOT OK (not proper type)" << *AI << "\n\n";
#endif  
    return false;
  }

#ifdef MYDEBUG
  if(true == AIType->isFPOrFPVectorTy()) {
    errs() << "\t\tIs FP or FP vector" << "\n";
  }
  if(true == AIType->isIntOrIntVectorTy()) {
    errs() << "\t\tIs Int or Int vector" << "\n";
  }
  if(true == AIType->isPtrOrPtrVectorTy()) {
    errs() << "\t\tIs Ptr or Ptr vector" << "\n";
  }
#endif  

  for (Value::use_iterator UI = AI->use_begin(), UE = AI->use_end(); 
        UI != UE; ++UI) {
    
    if (LoadInst *LI = dyn_cast<LoadInst>(*UI)) {
      if(isSafeLoad(LI)) {
        continue;
      }
    }
    if (StoreInst *SI = dyn_cast<StoreInst>(*UI)) {
      if(isSafeStore(SI, AI)) {
        continue;
      }
    }
#ifdef MYDEBUG
  errs() << "isAllocaPromotable : NOT OK (use other than load store)" << *AI << "\n\n";
#endif  
    return false;
  }

#ifdef MYDEBUG
  errs() << "isAllocaPromotable : OK" << *AI << "\n\n";
#endif  
  return true;
}

/*************************************************************************
 *  Function: SROA::isSafeLoad
 *  Purpose : Checks if the following are true for load LI  
 *            1) (not volatile).
 *
 ***********************************************************************/ 
bool SROA::isSafeLoad(LoadInst* LI)
{
#ifdef MYDEBUG
    errs() << "\t\tIs load:" << *LI << ":";
#endif

  if (LI->isVolatile()) {
#ifdef MYDEBUG
  errs() << "NO\n" ;
#endif  
      return false;
  } 
#ifdef MYDEBUG
  errs() << "YES\n" ;
#endif  
  return true;
}

/*************************************************************************
 *  Function: SROA::isSafeStore
 *  Purpose : Checks if the following is true for store SI  
 *            1) (no alloca instrucion 
 *               is used in the 'value' field of store) && (not volatile).
 *
 ***********************************************************************/ 
bool SROA::isSafeStore(StoreInst* SI, Value* AI)
{
#ifdef MYDEBUG
  errs() << "\t\tIs store:" << *SI << ":";
#endif

  if (SI->getOperand(0) == AI) {
#ifdef MYDEBUG
    errs() << "Is alloca used as data for store\n" ;
#endif
    return false; 
  }
  if (SI->isVolatile()) {
#ifdef MYDEBUG
  errs() << "NO\n" ;
#endif  
    return false;
  }
#ifdef MYDEBUG
  errs() << "YES\n" ;
#endif  
  return true;
}

/*************************************************************************
 *  Function: SROA::performScalarExpansion
 *  Purpose : Checks if the elements of the "Alloca" vector is expandible.
 *            If so, then expand it. This acticity may spawn more allocas
 *            which could be expanded or promoted to reg.
 *
 ***********************************************************************/ 
SmallVector<AllocaInst*, 32>* SROA::performScalarExpansion(
    SmallVector<AllocaInst*, 32> *Allocas, Function& F)
{
  SmallVector<AllocaInst*, 32> *newAllocas = new SmallVector<AllocaInst*, 32>();
  bool isExpanded = false;

  for(SmallVector<AllocaInst*, 32>::iterator AI = Allocas->begin(), 
      E = Allocas->end(); AI != E; ++AI)  {

    if(isAllocaExpandable(*AI)) {
      expandAlloca(*AI, newAllocas, F);
      isExpanded = true;
    }
  }

  if(true == isExpanded) {
    return newAllocas;
  } else {
    return NULL;
  }

}

/*************************************************************************
 *  Function: SROA::isAllocaExpandable
 *  Purpose : An alloca instrucion AI is expandible if all of the following is *            true:
 *            1) Allocate an object of a structure type.
 *            2) The uses of the alloca must abide by test_U1_or_U2
 *
 ***********************************************************************/ 
bool SROA::isAllocaExpandable(AllocaInst *AI) 
{
#ifdef MYDEBUG
  errs()<< "isAllocaExpandable Processing ... " << *AI << "\n";
#endif      

  if(!AI->getAllocatedType()->isStructTy()) {
#ifdef MYDEBUG
  errs()<< "\tisAllocaExpandable NOT OK (non struct) ... " << *AI << "\n";
#endif      
    return false;
  }

#ifdef MYDEBUG
  errs()<< "\tAlloca of struct test pass ... " << "\n";
#endif      

  /*
  if(AI->getNumUses() == 0) {
#ifdef MYDEBUG
  errs()<< "\tAlloca with no use ... " << "\n";
#endif      
    return false;
  }
  */

  for (Value::use_iterator UI = AI->use_begin(), UE = AI->use_end(); 
        UI != UE; ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    if(test_U1_or_U2(I)) {
#ifdef MYDEBUG
  errs()<< "\tAlloca Use test Pass on: " << *I << "\n\n";
#endif      
      continue;
    }
#ifdef MYDEBUG
  errs()<< "\tisAllocaExpandable NOT OK ... " << *AI << "\n";
#endif      
    return false;
  }

#ifdef MYDEBUG
  errs()<< "\tisAllocaExpandable OK ... " << *AI << "\n";
#endif      
  return true;
}

/*************************************************************************
 *  Function: SROA::test_U1_or_U2
 *  Purpose : Tests an instrucion I follows test_U1 or test_U2. 
 *
 ***********************************************************************/ 
bool SROA::test_U1_or_U2(Instruction *I)
{

#ifdef MYDEBUG
  errs()<< "\tProcessing Use: " << *I << "\n";
#endif      

  if(test_U1(I)) {
#ifdef MYDEBUG
    errs()<< "\t\ttest_U1_or_U2: U1 pass ... " << *I << "\n";
#endif      
    return true;
  } 
#ifdef MYDEBUG
    else {
      errs()<< "\t\ttest_U1_or_U2: U1 fail ... " << *I << "\n";
    }
#endif      


  if(test_U2(I)) {
#ifdef MYDEBUG
    errs()<< "\t\ttest_U1_or_U2: U2 pass ... " << *I << "\n";
#endif      
    return true;
  }
#ifdef MYDEBUG
  else {
    errs()<< "\t\ttest_U1_or_U2: U2 fail ... " << *I << "\n";
  }
#endif      

  return false;
}

/*************************************************************************
 *  Function: SROA::test_U1
 *  Purpose : Tests if alloca instrucion AI follow all the conditions 
 *            below:
 *            1. The type should be getelementptr
 *            2. The instrucion should have a fixed format.
 *            2. The result of getelementptr instrucion should follow
 *                    test_U1 or
 *                    test_U2 or
 *                    isSafeLoad or
 *                    isSafeStore. 
 *
 ***********************************************************************/ 
bool SROA::test_U1(Instruction* AI)
{
#ifdef MYDEBUG
  errs() << "\t\ttesting U1" << "\n";  
#endif

  GetElementPtrInst* G = dyn_cast<GetElementPtrInst>(AI);
  if(!G) {  
    return false;
  }
#ifdef MYDEBUG
  errs() << "\t\t\tIs a GEP instruction" << "\n";  
#endif
  
  if(!checkFormat(G)) {
#ifdef MYDEBUG
  errs() << "\t\t\tFormat NOT OK " << "\n";  
#endif
    return false;
  }
#ifdef MYDEBUG
  errs() << "\t\t\tFormat OK " << "\n";  
#endif

  for (Value::use_iterator UI = G->use_begin(), UE = G->use_end(); 
        UI != UE; ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    
    if(test_U1_or_U2(I)) {
      continue;
    }
    
#ifdef MYDEBUG
  errs() << "\t\t\ttesting Ld/St  " << "\n";  
#endif
    if (LoadInst *LI = dyn_cast<LoadInst>(*UI)) {
      if(isSafeLoad(LI)) {
        continue;
      }
    }
    if (StoreInst *SI = dyn_cast<StoreInst>(*UI)) {
      if(isSafeStore(SI, G)) {
        continue;
      }
    }
    
    return false;
  }
  return true;
}

/*************************************************************************
 *  Function: SROA::checkFormat
 *  Purpose : Tests if getelementptr instrucion G is of the the format
 *            getelementptr ptr, 0, constant[, ... constant]
 *
 ***********************************************************************/ 
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

/*************************************************************************
 *  Function: SROA::test_U2
 *  Purpose : Tests if instrucion AI is of type comparison (’eq’ or ’ne’) 
 *            where the other operand is the NULL pointer value.
 *
 ***********************************************************************/ 
bool SROA::test_U2(Instruction* AI) 
{
#ifdef MYDEBUG
  errs() << "\t\ttesting U2" << "\n";  
#endif
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

void SROA::expandAlloca(AllocaInst *AI, SmallVector<AllocaInst*, 32>* newAllocas, Function& F) 
{
  NumReplaced ++;
  
  StructType *T = cast<StructType>(AI->getAllocatedType());
  
  for(unsigned i=0 ; i< T->getNumElements() ; i++) {
    
    Type *TT = T->getElementType(i);
    Instruction* First = F.getEntryBlock().begin();

    AllocaInst *ai = new AllocaInst(TT);
    //ai->insertBefore(AI);
    ai->insertBefore(First);
    
    replaceUses(AI, i, ai);
    
    if(TT->isStructTy()) {
#ifdef MYDEBUG
      errs() << "Adding new\n";
#endif
      newAllocas->push_back(ai);
    }
  }
}

bool SROA::replaceUses(Instruction* OrigInst, unsigned offset, Value* newValue) 
{
#ifdef MYDEBUG  
 errs() <<"=============" << "\n";;
 errs() <<"Orig Instruction:" << *OrigInst << "\n";;
 errs() <<"New Instruction:" << *newValue << "\n";;
 errs() <<"Offset:" << offset << "\n";;
 errs() <<"=============" << "\n";;
#endif 

  bool valueUsed = false;
  for (Value::use_iterator UI = OrigInst->use_begin(); UI != OrigInst->use_end();) {
    if (GetElementPtrInst *Inst = dyn_cast_or_null<GetElementPtrInst>(*UI)) {
      if(dyn_cast<ConstantInt>(Inst->getOperand(2))->getZExtValue() == offset) {
        BasicBlock::iterator BI(Inst);
        ++UI;
        //ReplaceInstWithValue(Inst->getParent()->getInstList(), BI, newValue);
        Inst->replaceAllUsesWith(newValue);
        Inst->eraseFromParent();
        valueUsed = true;
      } else {
        ++UI;
      }
    }
  }
  return valueUsed;
}



void SROA::cleanUnusedAllocas(Function &F) {
  BasicBlock &BB = F.getEntryBlock();
  for (BasicBlock::iterator I = (&BB)->begin(), E = (&BB)->end(); I != E; ) {
    if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {
      if(AI->getNumUses() == 0) {
        ++I;
        AI->eraseFromParent();
      } else {
        ++I;
      }
    } else {
      ++I;
    }
  }
}

