//
// Created by bernard on 8/1/18.
//

#ifndef PROJECT_INJECTFAULT_H
#define PROJECT_INJECTFAULT_H
#include <iostream>
#include <fstream>
#include "llvm/Pass.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Metadata.h"
#include "llvm/Support/Debug.h"
#include <cxxabi.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/MemoryDependenceAnalysis.h>
#include <llvm/Analysis/DependenceAnalysis.h>
#include "boost/algorithm/string.hpp"
#include "llvm/ADT/SmallVector.h"

class InjectFault {

public:
    /** Global variable*/
    llvm::GlobalVariable *globalDimensionArray;
    llvm::Value * runtimeclearGlobalDimensionArray;
    llvm::Value * runtimeinsertGlobalDimensionArray;
    llvm::Value * runtimeFlipBitOn16IntegerValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeFlipBitOn32IntegerValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeFlipBitOn64IntegerValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeFlipBitOnFloatValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeFlipBitOnDoubleValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeInitializeRandomIntegerArrayValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeInitializeRandomDoubleArrayValue; // This value is to be retrieve from runtime.c
    llvm::Value * runtimeInitializeRandomFloatArrayValue; // This value is to be retrieve from runtime.c
    llvm::Value *SRAND;

    llvm::Value *getRuntimeFlipBitOnIntegerValue() const;

    void setRuntimeFlipBitOnIntegerValue(llvm::Value *runtimeFlipBitOnIntegerValue);

    void initialize(llvm::Module &module);
    std::string demangle(std::string name);

    bool injectFaultOnIntegerValue(llvm::AllocaInst *allocaInst);
    bool injectFaultOnFloatValue(llvm::AllocaInst *allocaInst);
    bool injectFaultOnDoubleValue(llvm::AllocaInst *allocaInst);
    bool injectFaultOnArrayValue(llvm::AllocaInst *allocaInst);

    void collectReAssignmentInstruction(llvm::Instruction *instruction,llvm::SmallVector<llvm::Instruction*,128> *WorkList);
    llvm::Type* getDimension(llvm::Type *type,std::list<uint64_t > *a);
    int getTotalElement(std::list<uint64_t > a);


};


#endif //PROJECT_INJECTFAULT_H
