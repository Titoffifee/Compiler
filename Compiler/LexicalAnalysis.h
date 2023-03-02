#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

enum class Type {
    Special = 0,
    Ident,
    Def,
    Bool,
    Int,
    Float,
    Equal,
    Operation,
    Semicolon,
    Comma,
    Dot,
    LeftRoundBracket,
    RightRoundBracket,
    LeftSquareBracket,
    RightSquareBracket,
    LeftBrace,
    RightBrace,
    LeftAngleBracket,
    RightAngleBracket
};

struct Lexeme {
    Lexeme(Type type, int line, std::string value = "");
    bool operator==(Type type);
    bool operator!=(Type type);
    Type type_;
    std::string value_;
    int line_;
};

std::ostream& operator<< (std::ostream& out, Lexeme& lex);

std::vector<Lexeme> LexicalAnalysis();