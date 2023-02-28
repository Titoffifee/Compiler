#pragma once
#include "SyntacticAnalysis.h"

class NameSpace {
    NameSpace(NameSpace* pr);
    ~NameSpace();
    std::map<std::string, VariableType*>names_;
    NameSpace* pr_;
};