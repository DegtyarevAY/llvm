add_llvm_library( LLVMBranchInstCount
  BranchInstCount.cpp
  BICinit.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
