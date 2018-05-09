//
// Created by bernard on 6/12/17.
//

#ifndef PROJECT_VARIABLEINFO_H
#define PROJECT_VARIABLEINFO_H
#define DEBUG_TYPE "Bernard's Dynamic Count"

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

class VariableInfo {
// This information will handle multiple overload function, need to know the function signature also... limiting for now...
 private:
     std::string  functionName;
     std::string variableName;


public:
    VariableInfo(const std::string &functionName, const std::string &variableName);

    const std::string &getFunctionName() const;

    void setFunctionName(const std::string &functionName);

    const std::string &getVariableName() const;

    void setVariableName(const std::string &variableName);


};


#endif //PROJECT_VARIABLEINFO_H
