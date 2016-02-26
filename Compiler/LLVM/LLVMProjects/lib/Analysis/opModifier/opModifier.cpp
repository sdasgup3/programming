#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/Instructions.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm; 

namespace {

  class opModifier: public FunctionPass {
public:
    static char ID;
    opModifier():FunctionPass(ID) {}
    bool runOnFunction(Function &M);
  };
}

char opModifier::ID=0;

static void registeropModifier(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new opModifier());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registeropModifier);


bool opModifier::runOnFunction(Function &F) {

  Function::iterator itB = F.begin();
  Function::iterator itE = F.end();

  for(; itB != itE; itB++) {
    BasicBlock *b = &(*itB);

    BasicBlock::iterator itb = b->begin();
    BasicBlock::iterator ite = b->end();

    for(; itb != ite; itb++) {
      Instruction *i = &(*itb);
      if(BinaryOperator* op = dyn_cast<BinaryOperator>(i)) {
        IRBuilder<> builder(op);

        // Make a multiply with the same operands as `op`.
        Value* lhs = op->getOperand(0);
        Value* rhs = op->getOperand(1);
        Value* mul = builder.CreateMul(lhs, rhs);

        op->replaceAllUsesWith(mul);  
      }
    }

  }



  return true;
}
