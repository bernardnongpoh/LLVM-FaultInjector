//
// Created by bernard on 6/12/17.
//

#include "VariableInfo.h"


const std::string &VariableInfo::getFunctionName() const {
    return functionName;
}

void VariableInfo::setFunctionName(const std::string &functionName) {
    VariableInfo::functionName = functionName;
}

const std::string &VariableInfo::getVariableName() const {
    return variableName;
}

void VariableInfo::setVariableName(const std::string &variableName) {
    VariableInfo::variableName = variableName;
}

VariableInfo::VariableInfo(const std::string &functionName, const std::string &variableName) : functionName(
        functionName), variableName(variableName) {}

 std::string &VariableInfo::getKey()  {
    key=key+functionName+"_"+variableName;
    return key;
}
