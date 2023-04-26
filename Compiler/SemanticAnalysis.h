#pragma once
#include <map>
#include "LexicalAnalysis.h"
#include "ExceptionsSemantic.h"

class VariableType;

class NameSpace {
public:
    NameSpace(NameSpace* pr = nullptr);
    ~NameSpace();
    void Add(Lexeme& lexeme_name, VariableType* type);
    VariableType* GetVariableType(Lexeme& lexeme_name);
private:
    bool IsInSpace(std::string& name);
    std::map<std::string, VariableType*>names_;
    NameSpace* pr_;
};

enum class VariableTypes {
    Bool = 0,
    Int,
    Float,
    Array
};

class VariableType {
public:
    VariableType(VariableTypes type);
    VariableType(VariableType* next);
    VariableType(Lexeme& type);
    VariableType(Type type);
    ~VariableType();
    bool operator==(VariableType other);
    bool operator!=(VariableType other);
    bool operator>(VariableType other);
    bool IsBaseType();
    bool IsArray();
    VariableType* Next();
    VariableType* GetFullCopy();
private:
    VariableType* next_;
    VariableTypes type_;
};

class FunctionParameter {
public:
    FunctionParameter(VariableType* value, 
        FunctionParameter* next = nullptr);
    ~FunctionParameter();
    bool operator==(FunctionParameter other);
    bool operator!=(FunctionParameter other);
private:
    FunctionParameter* next_;
    VariableType* value_;
};

class FunctionNameSpace {
public:
    FunctionNameSpace(FunctionNameSpace* pr = nullptr);
    ~FunctionNameSpace();
    void AddFunction(Lexeme* lexeme_name,
        VariableType* value_type, 
        FunctionParameter* parametr);
    // bool IsInSpace(std::string name);
    VariableType* GetFunctionType(Lexeme* lexeme_name);
    FunctionParameter* GetFunctionParametrs(Lexeme* lexeme_name);
private:
    std::map<std::string, VariableType*>return_value_;
    std::map<std::string, FunctionParameter*>parametrs_;
    FunctionNameSpace* pr_;
};

VariableType* CheckBinExpression(VariableType* first, VariableType* second, int line);
VariableType* CheckUnoExpression(VariableType* type, int line);
VariableType* GetVariableType(Lexeme& variable_name, NameSpace* name_space);
void CheckCanDoEqual(VariableType* left, VariableType* right, int line);
void CheckIsResultBasedAndDelete(VariableType* result, int line);