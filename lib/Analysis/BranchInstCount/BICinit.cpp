#include "llvm/InitializePasses.h"
#include "llvm-c/Initialization.h"
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

/// llvm::initializeBIC - Initialize all passes in the BIC library
void llvm::initializeBIC(PassRegistry &Registry) 
{
  initializeBranchInstCountPass(Registry);
}

/// LLVMInitializeBIC - C binding for llvm::initializeBIC 
void LLVMInitializeBIC(LLVMPassRegistryRef R) 
{
  llvm::initializeBIC(* llvm::unwrap(R));
}