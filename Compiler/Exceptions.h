#pragma once
#include <string>
#include "LexicalAnalysis.h"

struct Lexeme;

class ExceptionInRow {
public:
    ExceptionInRow(std::string text, Lexeme* lexeme);
    virtual ~ExceptionInRow() {}
    virtual void print() = 0;
protected:
    std::string text_;
    Lexeme* lexeme_;
};

class ExceptionInLexeme : public ExceptionInRow {
public:
    ExceptionInLexeme(std::string text, Lexeme* lexeme);
    virtual void print();
};