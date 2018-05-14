/* This fault injector is for Bayesian Fault Injector */
#define DEBUG_TYPE "Bernard's Fault Injector for Bayesian Fault Model"
#include <iostream>
#include <fstream>
#include "llvm/Pass.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/Support/Debug.h"
#include <cxxabi.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/MemoryDependenceAnalysis.h>
#include <llvm/Analysis/DependenceAnalysis.h>
#include "boost/algorithm/string.hpp"
#include "VariableInfo.h"
#include "FaultEngine.h"

using namespace llvm;
namespace {
    class FaultInjector: public ModulePass{
    public:

        InjectFault *injectFault;
        // This is a list of variable for fault injection.
        std::list<VariableInfo*> faultTargetVars;
        std::map<std::string,VariableInfo*> faultTargetVarsMap;
        // Map to Instruction Pointer Here
        // Can we have map from function --> variables Here
        std::map<std::string,Instruction*> instMap;
        static char ID;
        FaultInjector():ModulePass(ID){

        }
       bool runOnModule(Module &M);
        bool runOnFunction(Function &F);
        std::string demangle(std::string name);

        bool doInitialization(Module &module) override;

        bool doFinalization(Module &module) override;

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }
    };
}





std::string FaultInjector::demangle(std::string name) {
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

bool FaultInjector::runOnModule(Module &M) {

    for(Module::iterator F=M.begin(), E = M.end();F!=E;++F){
        runOnFunction(*F);
    }

    return false;
}

bool FaultInjector::runOnFunction(Function &F) {


    bool  isModified= false;

    std::string functionName = demangle(F.getName().str());

    // Blacklist function : Skip instrumentation on the following functions, need to shift to a file and proper data structure
    if(functionName.compare("srand")==0 || functionName.compare("SRAND")==0 || functionName.compare("LOAD")==0 || functionName.compare("main")==0  || functionName.compare("STORE")==0 || functionName.compare("injectError32IntData")==0 ||  functionName.compare("PRINT")==0  ||  functionName.compare("flipBitOnIntegerValue") ==0|| functionName.compare("randr")==0 || functionName.compare("flipBits")==0 || functionName.compare("srand")==0 || functionName.compare("rand")==0 || functionName.compare("time")==0 || functionName.compare("llvm.dbg.declare")==0) return false;


    LLVMContext &Context = F.getContext();


    // This point to the start of the basic block


    /** Get the argument List here */
    for(auto arg = F.arg_begin(); arg != F.arg_end(); ++arg) {

        VariableInfo *variableInfo=new VariableInfo(functionName,arg->getName());

        if(faultTargetVarsMap.find(variableInfo->getKey())==faultTargetVarsMap.end()){
            // Not found, need not instrument , instrument only the target variable
            continue;
        }


        for(auto &B:F){
            for(auto &I:B){
                if(arg->getType()->isIntegerTy())
                {
                    injectFault->injectFaultOnParamIntegerValue(arg,&I);

                }
                break;
            }

            break;
        }


        // check the function name and so on for further analysis. even we can check at the function level.
        // Todo : need to follow this parameter




    }

    for(auto &B:F){
        for(auto &I:B){

            if (AllocaInst *allocaInst = dyn_cast<AllocaInst>(&I)) {

                // There is a problem here : either tracking by allocaInst or by dbgDeclareInst.

                std::vector<Value*> args;
                VariableInfo *variableInfo=new VariableInfo(functionName,allocaInst->getName());

                if(faultTargetVarsMap.find(variableInfo->getKey())==faultTargetVarsMap.end()){
                    // Not found, need not instrument , instrument only the target variable
                    continue;
                }

                allocaInst->dump();



                // Look-up for variable in the list... maybe we can have a  hash look up functionname+variablename as the hashkey for fast searching
                // Need to inject error in LOAD and STORE of This variable if it is a target variable

               // 1. Store random value and removing all store instruction to this variable.

                auto type=allocaInst->getAllocatedType();
                type->dump();

                if(type->isIntegerTy() && !type->isPointerTy()){


                    isModified=injectFault->injectFaultOnIntegerValue(allocaInst);
        // inject error on integer type need to check for 32 and 64 bit
                }
                else if(type->isFloatTy() && !type->isPointerTy()){
                    isModified=injectFault->injectFaultOnFloatValue(allocaInst);
                }
                else if(type->isDoubleTy() && !type->isPointerTy()){
                    isModified=injectFault->injectFaultOnDoubleValue(allocaInst);
                }
                // This is for long double type
                else if(type->isX86_FP80Ty()){
                    isModified=injectFault->injectFaultOnDoubleValue(allocaInst);
                }

                else if(type->isArrayTy() && !type->isPointerTy()){
                    isModified=injectFault->injectFaultOnArrayValue(allocaInst);
                }




            }

        }

    }


    // return true if change made to IR else false

    return isModified;

}





bool FaultInjector::doFinalization(Module &module) {
    // Here write to a file
    std::ofstream myfile;
    myfile.open ("varinfo.out");


    for(VariableInfo *variableInfo: faultTargetVars){
        //myfile <<"<variable functionName=\""<<variableInfo->getFunctionName()<<"\" variableName=\""<<variableInfo->getVariableName()<<"\"/>\n";
    }

    myfile.close();

    return true;
}

bool FaultInjector::doInitialization(Module &module) {

    injectFault=new InjectFault(); // Need to initialize this one

    injectFault->initialize(module);


    // Initialize target fault injection for variable, Move this one to  read from file temp as for now
    VariableInfo *variableInfo=new VariableInfo("hello","a");
    // Insert into the maxp
    faultTargetVarsMap[variableInfo->getKey()]=variableInfo;

    for(std::pair<std::string,VariableInfo*> pair: faultTargetVarsMap){
        errs()<<pair.first;

    }


    return Pass::doInitialization(module);
}


char FaultInjector::ID = 0;
static RegisterPass<FaultInjector> X("FaultInjector","LLVM fault Injector",false, true);




