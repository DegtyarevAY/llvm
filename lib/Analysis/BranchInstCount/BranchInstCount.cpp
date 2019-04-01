#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/BranchInstCount/BICinit.h"
#include "llvm/Pass.h"

using namespace llvm;

#define DEBUG_TYPE "BranchInstCount"

// Branch Instruction Counting
STATISTIC(NumCondBranch, "Number of conditional branches in the program");
STATISTIC(NumUncondBranch, "Number of unconditional branches in the program");
STATISTIC(NumEqBranch, "Number of conditional branches whose comparison type is equal test");
STATISTIC(NumGTBranch, "Number of conditional branches whose comparison type is greater than test");
STATISTIC(NumLTBranch, "Number of conditional branches whose comparison type is less than test");

namespace 
{
class BranchInstCount : public BasicBlockPass 
{
  void conditionalBranchAnalyze(CmpInst *instruction) 
  {
	if (!instruction)
		return;

	switch (instruction->getPredicate()) 
	{
		case CmpInst::FCMP_OGT:
		case CmpInst::FCMP_OGE:
		case CmpInst::FCMP_UGT:
		case CmpInst::FCMP_UGE:

		case CmpInst::ICMP_UGT:
		case CmpInst::ICMP_UGE:
		case CmpInst::ICMP_SGT:
		case CmpInst::ICMP_SGE:
			++NumGTBranch;
			break;
		case CmpInst::FCMP_OLT:
		case CmpInst::FCMP_OLE:
		case CmpInst::FCMP_ULT:
		case CmpInst::FCMP_ULE:

		case CmpInst::ICMP_ULT:
		case CmpInst::ICMP_ULE:
		case CmpInst::ICMP_SLT:
		case CmpInst::ICMP_SLE:
			++NumLTBranch;
			break;
		case CmpInst::FCMP_OEQ:
		case CmpInst::FCMP_UEQ:
		case CmpInst::ICMP_EQ:
			++NumEqBranch;
			break;
		default:
			break;
	}
  }
  void branchAnalyze(BranchInst *instruction) 
  {
    if (!instruction)
      return;

    if (instruction->isConditional()) 
	{
      ++NumCondBranch;
      conditionalBranchAnalyze(dyn_cast<CmpInst>(instruction->getCondition()));
    } 
	else
      ++NumUncondBranch;
  }

public:
  static char ID;
  BranchInstCount() : BasicBlockPass(ID) 
  {
    initializeBranchInstCountPass(*PassRegistry::getPassRegistry());
  }

  bool runOnBasicBlock(BasicBlock &BB) override 
  {
    for (Instruction &I : BB)
      branchAnalyze(dyn_cast<BranchInst>(&I));
    return false;
  }

  virtual void print(raw_ostream &out, const Module *m) const override 
  {
    out << "NumCondBranch: " << NumCondBranch << "\n";
    out << "NumUncondBranch: " << NumUncondBranch << "\n";
    out << "NumEqBranch: " << NumEqBranch << "\n";
    out << "NumGTBranch: " << NumGTBranch << "\n";
    out << "NumLTBranch: " << NumLTBranch << "\n";
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override 
  {
    AU.setPreservesAll();
  }
};
} 

char BranchInstCount::ID = 0;

INITIALIZE_PASS(BranchInstCount, "BranchInstCount", "BranchInstCount Pass", true, true)

BasicBlockPass *llvm::createBranchInstCountPass() 
{
  return new BranchInstCount();
}
