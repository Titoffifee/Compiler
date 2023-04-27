#pragma once
#include "ExceptionsSyntactic.h"

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

class ExceptionWrongExpressionType : public Exception {
public:
    ExceptionWrongExpressionType(int line);
    virtual void print();
private:
    int line_;
};

class ExceptionWrongExpressionResult : public Exception {
public:
    ExceptionWrongExpressionResult(int line);
    virtual void print();
private:
    int line_;
};

class ExceptionWaitingArray : public Exception {
public:
    ExceptionWaitingArray(Lexeme* lexeme);
    virtual void print();
};

class ExceptionArrayEnd : public Exception {
public:
    ExceptionArrayEnd(Lexeme* lexeme);
    virtual void print();
};

class ExceptionArrayInitFinalCellNotBasic : public Exception {
public:
    ExceptionArrayInitFinalCellNotBasic(int line);
    virtual void print();
private:
    int line_;
};

class ExceptionActionWithVoidFunction : public Exception {
public:
    ExceptionActionWithVoidFunction(int line);
    virtual void print();
private:
    int line_;
};