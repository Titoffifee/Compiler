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
    RightBrace,
    LeftAngleBracket,
    RightAngleBracket
};

struct Lexeme {
    Lexeme();
    Lexeme(Type type, std::string value = "");
    bool operator==(Type type);
    bool operator!=(Type type);
    Type type_;
    std::string value_;
};

std::vector<Lexeme> LexicalAnalysis();