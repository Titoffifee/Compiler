#pragma once
#include <map>
#include "LexicalAnalysis.h"
#include "Exceptions.h"

class ExceptionVariableRedeclaration : public Exception {
public:
    ExceptionVariableRedeclaration(Lexeme* lexeme);
    virtual void print();
};

class ExceptionFunctionRedeclaration : public Exception {
public:
    ExceptionFunctionRedeclaration(Lexeme* lexeme);
    virtual void print();
};

class ExceptionVariableUndeclared : public Exception {
public:
    ExceptionVariableUndeclared(Lexeme* lexeme);
    virtual void print();
};

class ExceptionFunctionUndeclared : public Exception {
public:
    ExceptionFunctionUndeclared(Lexeme* lexeme);
    virtual void print();
};

class VariableType;

class NameSpace {
public:
    NameSpace(NameSpace* pr = nullptr);
    ~NameSpace();
    void Add(Lexeme* lexeme_name, VariableType* type);
    bool IsInSpace(std::string name);
    VariableType* GetVariableType(Lexeme* lexeme_name);
private:
    std::map<std::string, VariableType*>names_;
    NameSpace* pr_;
};

enum class VariableTypes {
    Int = 0,
    Float,
    Bool,
    Array
};

class VariableType {
public:
    VariableType(VariableTypes type);
    VariableType(VariableType* next);
    ~VariableType();
    bool operator==(VariableType other);
    bool operator!=(VariableType other);
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
    bool IsInSpace(std::string name);
    VariableType* GetFunctionType(Lexeme* lexeme_name);
    FunctionParameter* GetFunctionParametrs(Lexeme* lexeme_name);
private:
    std::map<std::string, VariableType*>return_value_;
    std::map<std::string, FunctionParameter*>parametrs_;
    FunctionNameSpace* pr_;
};