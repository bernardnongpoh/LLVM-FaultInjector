#define DEBUG_TYPE "Bernard's Dynamic Count"
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
#include "VariableInfo.h"
using namespace llvm;
namespace {
    class CountStats: public ModulePass{
    public:
        Value* func_corruptIntData_32bit;
        Value * func_corruptStore;
        std::list<VariableInfo*> variableInfoList;

        // Map to Instruction Pointer Here
        // Can we have map from function --> variables Here
        std::map<std::string,Instruction*> instMap;
        static char ID;
        CountStats():ModulePass(ID){

        }
       bool runOnModule(Module &M);
        bool runOnFunction(Function &F);
        std::string demangle(std::string name);



        bool doFinalization(Module &module) override;

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }
    };
}



char CountStats::ID = 0;
static RegisterPass<CountStats> X("DynCount","Count Statistics",false, true);







std::string CountStats::demangle(std::string name) {
    int status;
    std::string demangled;
    char* tmp = abi::__cxa_demangle(name.c_str(), NULL, NULL, &status);
    if (tmp == NULL)
        return name;

    demangled = tmp;
    free(tmp);
    /* drop the parameter list as we only need the function name */
    return demangled.find("(") == std::string::npos ? demangled : demangled.substr(0, demangled.find("("));

}

bool CountStats::runOnModule(Module &M) {

    for(Module::iterator F=M.begin(), E = M.end();F!=E;++F){
        runOnFunction(*F);
    }

    return false;
}

bool CountStats::runOnFunction(Function &F) {



    std::string functionName = demangle(F.getName().str());


    LLVMContext &Context = F.getContext();



    for(auto &B:F){
        for(auto &I:B){

            if (DbgDeclareInst *dbgDeclare = dyn_cast<DbgDeclareInst>(&I)) {

                VariableInfo *variablein=new VariableInfo(functionName,dbgDeclare->getVariable()->getName());
                variableInfoList.push_back(variablein);


            }
        }

    }




    return false;

}





bool CountStats::doFinalization(Module &module) {
    // Here write to a file hahaha
    std::ofstream myfile;
    myfile.open ("varinfo.out");


    for(VariableInfo *variableInfo: variableInfoList){
        myfile <<"<variable functionName=\""<<variableInfo->getFunctionName()<<"\" variableName=\""<<variableInfo->getVariableName()<<"\"/>\n";
    }

    myfile.close();

    return true;
}




