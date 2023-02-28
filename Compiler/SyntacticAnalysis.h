#pragma once
#include "LexicalAnalysis.h"
#include <map>

enum class VariableTypes {
    Int = 0, 
    Float, 
    Bool,
    Array
};

class VariableType {
public:
    VariableType(VariableTypes type);
    ~VariableType();
    bool operator==(VariableType* other);
    void Add(VariableTypes type); // вызывать исключение если приписываем не к array
    VariableTypes GetType();
private:
    VariableType* next_;
    VariableTypes type;
};

class FunctionParametr {
public:
    FunctionParametr();
    ~FunctionParametr();
    bool operator==(FunctionParametr* other);
    void AddParametr(VariableType* value);
private:
    FunctionParametr* next_;
    VariableType* value_;
};

class FunctionNames {
public:
    FunctionNames();
    ~FunctionNames();
    void AddFunction(std::string name, 
        VariableType* value_type, FunctionParametr* parametr);
    bool IsInSpace(std::string name);
    VariableType* FunctionType(std::string name);
    FunctionParametr* FunctionParametrs(std::string name);
private:
    std::map<std::string, VariableType*>return_value_;
    std::map<std::string, FunctionParametr*>parametrs_;
};

void SyntaciticAlalysis(std::vector<Lexeme>& lexemes);