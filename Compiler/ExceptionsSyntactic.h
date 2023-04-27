#pragma once
#include <string>
#include "LexicalAnalysis.h"

struct Lexeme;

class Exception {
public:
    Exception(std::string text, Lexeme* lexeme);
    virtual ~Exception() {}
    virtual void print();
protected:
    std::string text_;
    Lexeme* lexeme_;
};

class ExceptionFunctionName : public Exception {
public:
    ExceptionFunctionName(Lexeme* lexeme);
    virtual void print();
};

class ExceptionLeftRoundBracket : public Exception {
public:
    ExceptionLeftRoundBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionLeftBrace : public Exception {
public:
    ExceptionLeftBrace(Lexeme* lexeme);
    virtual void print();
};

class ExceptionLeftSquareBracket : public Exception {
public:
    ExceptionLeftSquareBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionLeftAngleBracket : public Exception {
public:
    ExceptionLeftAngleBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionRightRoundBracket : public Exception {
public:
    ExceptionRightRoundBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionRightBrace : public Exception {
public:
    ExceptionRightBrace(Lexeme* lexeme);
    virtual void print();
};

class ExceptionRightSquareBracket : public Exception {
public:
    ExceptionRightSquareBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionRightAngleBracket : public Exception {
public:
    ExceptionRightAngleBracket(Lexeme* lexeme);
    virtual void print();
};

class ExceptionVariableName: public Exception{
public:
    ExceptionVariableName(Lexeme* lexeme);
    virtual void print();
};

class ExceptionSemicolon : public Exception {
public:
    ExceptionSemicolon(Lexeme* lexeme);
    virtual void print();
};

class ExceptionSpecial : public Exception {
public:
    ExceptionSpecial(std::string word, Lexeme* lexeme);
    virtual void print();
};

class ExceptionType : public Exception {
public:
    ExceptionType(Lexeme* lexeme);
    virtual void print();
};

class ExceptionArray : public Exception {
public:
    ExceptionArray(Lexeme* lexeme);
    virtual void print();
};