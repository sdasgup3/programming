//===-------------- Test.cpp - test Code  --------===//
//
// The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//
#define LLVM_VERSION_MAJOR 3
#define LLVM_VERSION_MINOR 4
#define LLVM_VERSION(major,minor) (((major) << 2) | (minor))
#define LLVM_VERSION_CODE LLVM_VERSION(LLVM_VERSION_MAJOR,LLVM_VERSION_MINOR) 


#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/system_error.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/PassManager.h"
#include "llvm/Target/TargetLibraryInfo.h"

#if LLVM_VERSION_CODE >= LLVM_VERSION(3,5)
  #include "llvm/IR/Dominators.h"
  #include "llvm/IR/InstIterator.h"
#else
  #include "llvm/Analysis/Dominators.h"
  #include "llvm/Support/InstIterator.h"
#endif  
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

struct Test : public ModulePass {
  static char ID;
  Test() : ModulePass(ID) {}
  bool runOnModule(Module &M);

  void performTest1();
  void performTest2();
  void performTest3(Module &M);
  bool isLLVMPAPtrTy(llvm::Type*);

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
#if LLVM_VERSION_CODE >= LLVM_VERSION(3,5)
   // AU.addRequired<DominatorTreeWrapperPass>();
#else
   // AU.addRequired<DominatorTree>();
#endif
    AU.setPreservesCFG();
  }
  private:
    Module* Mod;
    const llvm::DataLayout *DL;
};

char Test::ID = 0;
static RegisterPass<Test> X("test",
"DSAND Testing Code ",
false /* does not modify the CFG */,
false /* transformation, not just analysis */);

/*******************************************************************
* Function : runOnModule
* Purpose : Entry point for Test
********************************************************************/
SwitchInst* GSI ; 

bool Test::runOnModule(Module &M)
{
  Mod = &M;
  //DL = Mod->getDataLayout();
  //DEBUG(errs() << "Data Layout String\n" << Mod->getDataLayoutStr() << "\n");

  bool Changed = false;

  //performTest1();
  //performTest2();
  llvm::errs() << "Performing  ITREAD\n";
  performTest3( M);

  return Changed;
}

void Test::performTest3(Module &mainModule) {
  /*
  OwningPtr<MemoryBuffer> BufferPtr;
  const char* file = "test.bc";
  std::string ErrorMsg;
  error_code ec=MemoryBuffer::getFileOrSTDIN(file, BufferPtr);
  if (ec) {
    llvm::errs() << "error loading program " << file << " " << ec.message().c_str();
  }
  Module* mainModule = getLazyBitcodeModule(BufferPtr.get(), getGlobalContext(), &ErrorMsg);
  if (mainModule) {
    if (mainModule->MaterializeAllPermanently(&ErrorMsg)) {
      delete mainModule;
      mainModule = 0;
    }
  }
  */

  //assert(mainModule);
  llvm::errs() << mainModule ;

  PassManager passes;
  llvm::DominatorTree *DT = new llvm::DominatorTree();
  passes.add(DT);

  passes.run(mainModule);

  if (DT->getRoot())
    llvm::errs() << DT->getRoot();

  
}


void Test::performTest2()
{
  for (Module::iterator FuncI = Mod->begin(), FuncE = Mod->end(); FuncI != FuncE; ++FuncI) {
    Function* Func = FuncI;
  
    for (Function::iterator BBI = Func->begin(), BBE = Func->end(); BBI != BBE; ++BBI) {
      BasicBlock* BB = BBI;
      for (BasicBlock::iterator II = BB->begin(), EI = BB->end(); II != EI; ++II) {
        Instruction* I = II;
        if(I->getOpcode() == Instruction::Call) { 
          DEBUG(errs() << "\n\n\nThis is Call Instrcution  \n" << *I<< "\n" );
          CallInst* CallI = cast<CallInst>(I);
          Function *fp = CallI->getCalledFunction();
          DEBUG(errs() << "Called Function " << fp->getName() << "\n");
          DEBUG(errs() << "Used in\n" );

          for (Value::use_iterator i = I->use_begin(), e = I->use_end(); i != e; ++i) {
            Instruction* UseI = dyn_cast<Instruction>(*i);
            if(isa<SwitchInst>(UseI)) {
              SwitchInst *SI = cast<SwitchInst>(UseI);
              if(fp->getName() == "F") {
                GSI = SI;
                DEBUG(errs() <<  *UseI<< "\n" );
              } else if(fp->getName() == "G") {
                if(SI != GSI) {
                  DEBUG(errs() <<  "Different Swict" << "\n" );
                }
              }
            }
          }

        }
      }
    }
  }
}

//for (Value::use_iterator i = F->use_begin(), e = F->use_end(); i != e; ++i)
//  if (Instruction *Inst = dyn_cast<Instruction>(*i)) {
//    errs() << "F is used in instruction:\n";
//    errs() << *Inst << "\n";
//  }
//










bool Test::isLLVMPAPtrTy(Type *Ty) {
  //Type *IntPtrTy = DL->getIntPtrType(Ty->getContext());
  //DEBUG(errs() << "\tType: " << *IntPtrTy << " Arg Type"<< *Ty  <<" \n");
  //return Ty->isPointerTy() || Ty == IntPtrTy;
  return 0;
}


void Test::performTest1()
{

  for (Module::iterator FI = Mod->begin(), FE = Mod->end(); FI != FE; ++FI) {
    Function* Func = FI;
    //DEBUG(errs() << *Func << "\n");
    for (Function::iterator BI = Func->begin(), BE = Func->end(); BI != BE; ++BI) {
      BasicBlock* BB = BI;
      for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ++I) {
        Instruction* BBI = I;
        if (StoreInst *SI = dyn_cast<StoreInst>(BBI)) {
          DEBUG(errs() << "\tStore Instruction: " << *BBI << " \n");
          DEBUG(errs() << "\t\tPointerType: " << isLLVMPAPtrTy(SI->getType()) << " \n");
          //Instruction* V = cast<Instruction>(SI->getOperand(1));
          //DEBUG(errs() << "\tOperand : " << *V << " \n");
          //DEBUG(errs() << "\t\tPointerType: " << isLLVMPAPtrTy(V->getType()) << " \n");
        } else {
          DEBUG(errs() << "\tInstruction: " << *BBI << " \n");
          DEBUG(errs() << "\t\tPointerType: " << isLLVMPAPtrTy(BBI->getType()) << " \n");
        }
      }
    }
  }
}
