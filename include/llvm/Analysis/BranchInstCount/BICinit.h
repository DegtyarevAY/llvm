#include "llvm/Pass.h"

#ifndef LLVM_BIC_H
#define LLVM_BIC_H

namespace llvm {

BasicBlockPass *createBranchInstCountPass();
}

#endif
