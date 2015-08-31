#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constants.h"



using namespace llvm;


namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct ADDKMS : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    ADDKMS() : ModulePass(ID) {}

    bool hasPointerType(Type *);

    virtual bool runOnModule(Module &M) {

      DataLayout *TD = getAnalysisIfAvailable<DataLayout>();
      Function* mainFunc = M.getFunction("main");

      llvm::Type *i8Ty = Type::getInt8Ty(getGlobalContext());
      Constant *fc = M.getOrInsertFunction("klee_make_symbolic",  Type::getVoidTy(getGlobalContext()),  
                                                                  PointerType::getUnqual(i8Ty), 
                                                                  Type::getInt64Ty(getGlobalContext()), 
                                                                  PointerType::getUnqual(i8Ty), NULL);
      Function* kleeMakeSymbolic = cast<Function>(fc);


      for(Module::global_iterator IG = M.global_begin(), EG = M.global_end(); IG != EG ; IG++ ) {
        GlobalVariable* gv = &*IG;  


        Type* g_type = gv->getType();
        Type* g_eltype = g_type->getPointerElementType();   
        DEBUG(errs() << "Global Vars: " << *gv << " Type: " << *g_type << " Elel Type"<< *g_eltype <<"\n" );

        if(gv->isConstant() && g_eltype->isArrayTy() && g_eltype->getArrayElementType()->isIntOrIntVectorTy()) {
          DEBUG(errs() << "Is Constant String" <<"\n" );
          continue;
        }

        if(true == hasPointerType(g_eltype)) {
          continue;
        }

        //lLLVM_TYPE_Q llvm::Type *i8Ty = Type::getInt8Ty(getGlobalContext());
        Type *VoidPtrTy = Type::getInt8PtrTy(getGlobalContext());
        Value* castInst = new BitCastInst(gv, VoidPtrTy, gv->getName() + ".aachecks", ++(mainFunc->begin()->begin()));

        std::vector<Value* > klee_args;
        klee_args.push_back(castInst);
        //klee_args.push_back(ConstantInt::get(Type::getInt64Ty(getGlobalContext()), TD->getTypeAllocSize(ld->getType())));
        klee_args.push_back(ConstantInt::get(Type::getInt64Ty(getGlobalContext()), TD->getTypeAllocSize(g_eltype)));
        klee_args.push_back(castInst);
        CallInst::Create(kleeMakeSymbolic, klee_args, "",(++(++(mainFunc->begin()->begin()))));
      }
      return false;
    }


    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
    }
  };
}

char ADDKMS::ID = 0;
static RegisterPass<ADDKMS> X("ADDKMS", "Add klee make symbolic");

bool ADDKMS::hasPointerType(Type* Ty) {

  if(Ty->isPointerTy()) {
    //DEBUG(errs() << "\tDereferenced: " << *loadInst << " Type: " <<    *(loadInst->getType())<< "\n" );
    //DEBUG(errs() << "\tDereferenced Type: " <<    *(g_eltype->getPointerElementType())<< "\n" );
    DEBUG(errs() << "Skipping for pointer type\n" );
    return true;
  }

  if(Ty->isArrayTy()) {
    Type* arrayElemTy  =  Ty->getArrayElementType();
    DEBUG(errs() << "Checking for pointer type in Array\n" );
    return hasPointerType(arrayElemTy);
  }

  if(Ty->isStructTy()) {
    StructType *STy = dyn_cast<StructType>(Ty);
    unsigned int numElemTy =  STy->getNumElements();
    DEBUG(errs() << "Checking for pointer type in Struct\n" );

    for(unsigned int  i = 0; i < numElemTy; i++ ) {
      Type* structElemType = STy->getElementType(i);
      if(true == hasPointerType(structElemType)) {
        return true;
      }
    }
    return false;
  }

  return false;
}
