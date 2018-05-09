//
// Created by Bernard Nongpoh on 8/1/18.
//

#include "FaultEngine.h"

bool InjectFault::injectFaultOnIntegerValue(llvm::AllocaInst *allocaInst) {
    bool ismModified= false;
    std::list<llvm::StoreInst*> storeInstList;

    for (llvm::Value::user_iterator i = allocaInst->user_begin(), e = allocaInst->user_end(); i != e; ++i) {
        // We only need StoreInst, if the the destination is the variable

        if (llvm::StoreInst *storeUsedInst = llvm::dyn_cast<llvm::StoreInst>(*i)) {
            llvm::errs()<<*storeUsedInst<<" Integer";
            storeInstList.push_back(storeUsedInst);
        }
    }

    if(storeInstList.size()!=0){
        std::vector<llvm::Value*> args;
        llvm::StoreInst *strInst = storeInstList.back();
        llvm::errs()<<*strInst<<"\n";
        args.push_back(strInst->getOperand(0));
        // Call fault injector runtime function here
        auto type=allocaInst->getAllocatedType();
        llvm::CallInst *callInst;

        if(type->isIntegerTy(16)){
            callInst = llvm::CallInst::Create(runtimeFlipBitOn16IntegerValue, args,
                                              "flipBitOn16IntegerValue", allocaInst->getNextNode());
        }


        if(type->isIntegerTy(32)) {

            callInst = llvm::CallInst::Create(runtimeFlipBitOn32IntegerValue, args,
                                                              "flipBitOn32IntegerValue", allocaInst->getNextNode());


        }
        else if(type->isIntegerTy(64)){
            callInst = llvm::CallInst::Create(runtimeFlipBitOn64IntegerValue, args,
                                              "flipBitOn64IntegerValue", allocaInst->getNextNode());
        }

            callInst->setCallingConv(llvm::CallingConv::C);


        for(llvm::StoreInst *str:storeInstList){
            // Set all First operand with the flipped value
            str->setOperand(0,callInst);

            ismModified=true;
        }


    }

    return ismModified;
}


bool InjectFault::injectFaultOnFloatValue(llvm::AllocaInst *allocaInst) {
    bool ismModified= false;
    std::list<llvm::StoreInst*> storeInstList;
    for (llvm::Value::user_iterator i = allocaInst->user_begin(), e = allocaInst->user_end(); i != e; ++i) {
        // We only need StoreInst, if the the destination is the variable

        if (llvm::StoreInst *storeUsedInst = llvm::dyn_cast<llvm::StoreInst>(*i)) {
            llvm::errs()<<*storeUsedInst<<" Float \n";
            storeInstList.push_back(storeUsedInst);

        }



    }

    if(storeInstList.size()!=0){
        std::vector<llvm::Value*> args;
        llvm::StoreInst *strInst = storeInstList.back();
        llvm::errs()<<*strInst<<"\n";
        args.push_back(strInst->getOperand(0));
        // Call fault injector runtime function here
        auto type=allocaInst->getAllocatedType();
        llvm::CallInst *callInst;

        if(type->isFloatTy()) {
            callInst = llvm::CallInst::Create(runtimeFlipBitOnFloatValue, args,
                                              "flipBitOnFloatValue", allocaInst->getNextNode());
        }

        callInst->setCallingConv(llvm::CallingConv::C);


        for(llvm::StoreInst *str:storeInstList){
            // Set all First operand with the flipped value
            str->setOperand(0,callInst);
            ismModified=true;
        }


    }

    return ismModified;
}

bool InjectFault::injectFaultOnDoubleValue(llvm::AllocaInst *allocaInst) {
    bool ismModified= false;
    std::list<llvm::StoreInst*> storeInstList;
    for (llvm::Value::user_iterator i = allocaInst->user_begin(), e = allocaInst->user_end(); i != e; ++i) {
        // We only need StoreInst, if the the destination is the variable

        if (llvm::StoreInst *storeUsedInst = llvm::dyn_cast<llvm::StoreInst>(*i)) {
            llvm::errs()<<*storeUsedInst<<" double \n";
            storeInstList.push_back(storeUsedInst);

        }



    }

    if(storeInstList.size()!=0){
        std::vector<llvm::Value*> args;
        llvm::StoreInst *strInst = storeInstList.back();
        llvm::errs()<<*strInst<<"\n";
        args.push_back(strInst->getOperand(0));
        // Call fault injector runtime function here
        auto type=allocaInst->getAllocatedType();
        llvm::CallInst *callInst;

        if(type->isDoubleTy()) {
            callInst = llvm::CallInst::Create(runtimeFlipBitOnDoubleValue, args,
                                              "flipBitOnDoubleValue", allocaInst->getNextNode());
        }

        callInst->setCallingConv(llvm::CallingConv::C);


        for(llvm::StoreInst *str:storeInstList){
            // Set all First operand with the flipped value
            str->setOperand(0,callInst);
            ismModified=true;
        }


    }

    return ismModified;
}

void InjectFault::collectReAssignmentInstruction(llvm::Instruction *instruction,llvm::SmallVector<llvm::Instruction*,128> *WorkList){


      for (llvm::Value::user_iterator i = instruction->user_begin(), e = instruction->user_end(); i != e; ++i) {



        if (llvm::GetElementPtrInst *getElementPtrInst = llvm::dyn_cast<llvm::GetElementPtrInst>(*i)) {

            //WorkList->push_back(getElementPtrInst);
            InjectFault::collectReAssignmentInstruction(getElementPtrInst, WorkList);
           //WorkList->push_back(getElementPtrInst); // Removing this one also

        }
        else if(llvm::StoreInst *storeInst = llvm::dyn_cast<llvm::StoreInst>(*i)){
            WorkList->push_back(storeInst);
        }

        else if (llvm::BitCastInst *bitCastInst = llvm::dyn_cast<llvm::BitCastInst>(*i)) {

            /*
             * You cannot change an iterator while iterating over it
            • To remove instructions, first collect the instructions to remove, then remove them in a separate pass
            • What does this example do?
             *
             */
            //Look for user of bitcast also, this will be call memcopy
           InjectFault::collectReAssignmentInstruction(bitCastInst, WorkList);

        }

               else if (llvm::CallInst *callInst = llvm::dyn_cast<llvm::CallInst>(*i)) {

                    if(callInst->getCalledFunction()->getName().compare("llvm.memcpy.p0i8.p0i8.i64")==0){
                        // Insert a call to runtime function here
                        /*removeFromParent() doesn't delete the instruction, it just removes it
                        from the basic block. This is useful if you want to re-insert it
                    somewhere else.
                    However, I think what you're looking for is eraseFromParent(), which
                    actually deletes the instruction after removing it from its parent.
                    This also takes care of removing it from the use lists of all of its
                    operands. */
                        //callInst->eraseFromParent();
                        //bitcastInst->eraseFromParent();
                      WorkList->push_back(callInst);

                    }

                        /// Todo: Need to look into various initialization using memset
            else if(callInst->getCalledFunction()->getName().compare("llvm.memset.p0i8.i64")==0){

                WorkList->push_back(callInst);

            }


        }




        }



    }

llvm::Type* InjectFault::getDimension(llvm::Type *type,std::list<uint64_t > *a){

    // This is an array Type
    if(llvm::ArrayType *Atype=llvm::dyn_cast<llvm::ArrayType>(type)){
       a->push_back(Atype->getNumElements());
        llvm::errs()<<"here "<<Atype->getNumElements();
        InjectFault::getDimension(Atype->getArrayElementType(),a);
    }
    else{
        return type;
    }





}

bool InjectFault::injectFaultOnArrayValue(llvm::AllocaInst *allocaInst) {
    bool ismModified= false;

    std::vector<llvm::Value*> args;
    auto type=allocaInst->getAllocatedType();
    type->dump();

    llvm::CallInst *callInst;
    llvm::SmallVector<llvm::Instruction*,128> *WorkList=new llvm::SmallVector<llvm::Instruction*,128>();
    /* Collect all use that re-assign array value, block re-assigment of value **/

    InjectFault::collectReAssignmentInstruction(allocaInst,WorkList);

    while(!WorkList->empty()){
        llvm::Instruction *I = WorkList->pop_back_val();
        I->eraseFromParent();
    }

    std::list<uint64_t > a;
    // The return is a type of the element in an array
    llvm::Type *ArrayElementType=InjectFault::getDimension(type,&a);
    //int totalElement = InjectFault::getTotalElement(a);

    //int size = (int) Atype->getNumElements()* Atype->getElementType()->getPrimitiveSizeInBits();
   // llvm::errs()<<"Size="<<size;

    // Need to Create an array and pass to the run time function



    llvm::Type *i32_type = llvm::IntegerType::getInt32Ty(allocaInst->getContext());

    llvm::Type *i64_type = llvm::IntegerType::getInt64Ty(allocaInst->getContext());

    llvm::Value *noOfDimension = llvm::ConstantInt::get(i32_type, a.size());


    //llvm::ArrayType *arrayType=llvm::ArrayType::get(i32_type,a.size());

    //llvm::Constant *constantList[a.size()];





    llvm::Value *indexList[1] ={llvm::ConstantInt::get(i64_type,0)};


    llvm::ArrayRef<llvm::Value*> arrayRef;


   // llvm::ArrayRef<llvm::Constant*> dimensionConstant(constantList,a.size());

   // llvm::Value *constantArray=llvm::ConstantArray::get(arrayType,dimensionConstant);

    /*  **/

    // TODO: This need to change based on type ..

    //llvm::Value *globalDimensionArrayBase = llvm::GetElementPtrInst::CreateInBounds(globalDimensionArray,arrayRef,"globalArrayIndex",allocaInst->getNextNode());
    //llvm::Instruction *instructionBase=llvm::dyn_cast<llvm::Instruction>(globalDimensionArrayBase);

    //llvm::CallInst *callInsertGlobalDA=llvm::CallInst::Create(runtimeclearGlobalDimensionArray,args,""
      //      , instructionBase->getNextNode());

    //callInsertGlobalDA->setCallingConv(llvm::CallingConv::C);

    //globalDimensionArrayBase->dump();





    llvm::Value *baseIndex = llvm::GetElementPtrInst::CreateInBounds(type,allocaInst,arrayRef,"indx",allocaInst->getNextNode());
    baseIndex->dump();
    llvm::Instruction *baseInstruction=llvm::dyn_cast<llvm::Instruction>(baseIndex);

// Goal is to put element into a global array, before that, call initialize to to clear all prev value.

    llvm::CallInst *clearGlobalDimensionArrayCall=llvm::CallInst::Create(runtimeclearGlobalDimensionArray,"",allocaInst->getNextNode());
    clearGlobalDimensionArrayCall->setCallingConv(llvm::CallingConv::C);


    // Create a call to initialized globalDimensionArray
    int i=0;
    llvm::Instruction *currentInst=llvm::dyn_cast<llvm::Instruction>(clearGlobalDimensionArrayCall);
    for(int ele:a){
        std::vector<llvm::Value*> arguments;
        arguments.clear();
        llvm::Value *data = llvm::ConstantInt::get(i32_type, ele);// element
        llvm::Value *index=llvm::ConstantInt::get(i32_type,i);
        arguments.push_back(data);
        arguments.push_back(index);
        llvm::CallInst *insertGlobalDimensionArrayCall=llvm::CallInst::Create(runtimeinsertGlobalDimensionArray,arguments,"",currentInst->getNextNode());
        insertGlobalDimensionArrayCall->setCallingConv(llvm::CallingConv::C);
        // Update the current Instruction
        currentInst=llvm::dyn_cast<llvm::Instruction>(insertGlobalDimensionArrayCall);
        i++;
    }

    // Get the pointer to the declare array and bitcast
    llvm::PointerType *pointerType=llvm::PointerType::get(ArrayElementType,0);
    llvm::Value *bitCastInst=llvm::BitCastInst::CreatePointerCast(baseInstruction,pointerType,"bitcaseArrayIdx",baseInstruction->getNextNode());
    bitCastInst->dump();

    llvm::Instruction *bitinstruction=llvm::dyn_cast<llvm::Instruction>(bitCastInst);

    args.clear();
    args.push_back(bitCastInst);
    args.push_back(noOfDimension);




    if(ArrayElementType->isIntegerTy()){
        callInst = llvm::CallInst::Create(runtimeInitializeRandomIntegerArrayValue, args,"", bitinstruction->getNextNode());
    }
    else if(ArrayElementType->isFloatTy()){
        callInst = llvm::CallInst::Create(runtimeInitializeRandomFloatArrayValue, args,""
                , bitinstruction->getNextNode());
    }
    else if(ArrayElementType->isDoubleTy()){
        callInst = llvm::CallInst::Create(runtimeInitializeRandomDoubleArrayValue, args,""
                , bitinstruction->getNextNode());
    }


  //  callInst->setCallingConv(llvm::CallingConv::C);











       //









    return ismModified;

}





std::string InjectFault::demangle(std::string name) {
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

void InjectFault::initialize(llvm::Module &module) {


    llvm::Function *Main = module.getFunction("main");
    assert(Main && "Error: Requires a main function");
    // Read from file, which variable to inject error
    // Hardcoded as for now
    // assume variable : a from function b
    llvm::LLVMContext &Context = module.getContext();

    for (auto F = module.getFunctionList().begin(), E = module.getFunctionList().end(); F != E; F++) {
        std::string cstr = InjectFault::demangle(F->getName().str());

        if (cstr.find("flipBitOn16IntegerValue") != std::string::npos) {

            runtimeFlipBitOn16IntegerValue=&*F;

        }

        if (cstr.find("flipBitOn32IntegerValue") != std::string::npos) {

            runtimeFlipBitOn32IntegerValue=&*F;

        }

        else if (cstr.find("flipBitOn64IntegerValue") != std::string::npos) {

            runtimeFlipBitOn64IntegerValue=&*F;

        }


        else if (cstr.find("flipBitOnFloatValue") != std::string::npos) {

            runtimeFlipBitOnFloatValue=&*F;

        }

        else if (cstr.find("flipBitOnDoubleValue") != std::string::npos) {

            runtimeFlipBitOnDoubleValue=&*F;

        }

        else if (cstr.find("initializeIntegerArray") != std::string::npos) {

            runtimeInitializeRandomIntegerArrayValue=&*F;

        }
        else if (cstr.find("initializeFloatArray") != std::string::npos) {

            runtimeInitializeRandomFloatArrayValue=&*F;

        }

        else if (cstr.find("initializeDoubleArray") != std::string::npos) {

            runtimeInitializeRandomDoubleArrayValue=&*F;

        }






        else if (cstr.find("SRAND") != std::string::npos) {
            SRAND = &*F;
        }


        else if (cstr.find("clearGlobalDimensionArray") != std::string::npos) {

            runtimeclearGlobalDimensionArray=&*F;

        }

        else if (cstr.find("insertGlobalDimensionArray") != std::string::npos) {

            runtimeinsertGlobalDimensionArray=&*F;

        }

    }

    // Setup to use srand(), this code inject error in the main function of a program to call srand()
    // Random number seed setting here

    llvm::BasicBlock &BB = Main->front();
    llvm::CallInst *callInst= llvm::CallInst::Create(SRAND,"",BB.front().getNextNode());
    callInst->setCallingConv(llvm::CallingConv::C);

    /* Retrieved some global scrach pad**/
    globalDimensionArray=module.getGlobalVariable("globalDimensionArray");



    // Get element pointer and pass to function to insert element

}

int InjectFault::getTotalElement(std::list<uint64_t> a) {
    int total=1;
    for (auto const& i : a) {
        total*=i;
    }

    return total;
}



