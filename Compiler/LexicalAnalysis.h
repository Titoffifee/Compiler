#pragma once

#include <iostream>
#include <fstream>
#include <vector>

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
    RightBrace
};

struct Lexeme {
    Lexeme();
    Lexeme(Type type_, std::string value_ = "");

    Type type;
    std::string value;
};

std::vector<Lexeme> LexicalAnalysis();