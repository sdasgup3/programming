#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/Instructions.h"

using namespace llvm; 

namespace {

  class gptr: public ModulePass {
public:
    static char ID;
    gptr():ModulePass(ID) {}
    bool runOnModule(Module &M);
  };
}

std::string getTypeName(Type *T) {
    std::string TypeName;
    raw_string_ostream TypeStream(TypeName);
    T->print(TypeStream);
    TypeStream.flush();
    return TypeName;
}


char gptr::ID=0;
static RegisterPass<gptr> X("gptr", "getElementPointer Pass", true, true);
bool gptr::runOnModule(Module &M) {

  for(Module::iterator mbit = M.begin(), meit = M.end() ; mbit != meit; mbit++) {
    Function *F =  mbit;
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
      Instruction &i = *I;
      if(i.getOpcode() == Instruction::GetElementPtr) {
        GetElementPtrInst *Gep = dyn_cast<GetElementPtrInst>(&i);
        //errs() << *Gep << "\n";
        Gep->dump();

        Value *pointer = Gep->getPointerOperand();
        Type* ptrType = pointer->getType();
        errs() << "\t\tPointer Operand (Type: " << getTypeName(ptrType) <<" )\t";
        pointer->dump();
        unsigned numI = Gep->getNumIndices(); 
        errs() << "\n\t\tNum Of Indices\t "<< numI  <<"\n";

        errs() << "\n\t\tIndices are\n ";
        for (User::op_iterator OPI = Gep->idx_begin(), OPE = Gep->idx_end(); OPI != OPE; ++OPI) {
          //errs() << OPI << "\n";    
          Value* v = *OPI;
          v->dump();
        }


        if(PointerType *pt = dyn_cast<PointerType> (ptrType)) {
          errs() << "\n\t\tFirst operand is of Pointer Type\n ";
          Type* elementType = pt->getElementType();
          errs() << "The element type is: " << *elementType << "\n";

                if (elementType->isArrayTy()) {
                    unsigned size = ((ArrayType *)elementType)->getNumElements();
                    errs() << "  .. points to an array! " << size << "\n";
                } else if(elementType->isStructTy()) {
                    unsigned size = ((StructType *)elementType)->getNumElements();
                    errs() << "  .. points to an Struct! " << size << "\n";
                } 

        }


      }
    }
  }

  return false;
}
