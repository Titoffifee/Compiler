#include "SyntacticAnalysis.h"

void ValueType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Special) {
        throw; // ќжидалс€ тип
    }
    if (lexemes[i].value_ == "int" || lexemes[i].value_ == "bool" || lexemes[i].value_ == "float") {
        ++i;
        return;
    }
    if (lexemes[i].value_ == "array") {
        ++i;
        if (lexemes[i] == Type::LeftAngleBracket) {
            ++i;
            ValueType(lexemes, i);
            if (lexemes[i] == Type::RightAngleBracket) {
                ++i;
                return;
            }
            throw; // ќжидалась закрывающа€ углова€ скобка
        } 
        throw; // ќжидалась открывающа€ углова€ скобка
    }
    throw; // ќжидалс€ тип
}

void Action(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] == Type::Special) {

    }
}

void Block(std::vector<Lexeme>& lexemes, int& i) {
    while (lexemes[i] != Type::RightBrace) {
        Block(lexemes, i);
        if (lexemes[i] != Type::Semicolon)
            throw; // Ќе хватает точки с за€птой
        ++i;
    }
    ++i;
}

void FunctionType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "void") {
        ++i;
        return;
    }
    ValueType(lexemes, i);
}

void FunctionValue(std::vector<Lexeme>& lexemes, int& i) {
    ValueType(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw; // ќжидалось им€ переменной
    ++i;
    while (lexemes[i] == Type::Comma) {
        ++i;
        if (lexemes[i] != Type::Ident) {
            throw; // ќжидалось им€ перечисл€емой переменной
        }
        ++i;
    }
}

void FucntionParameters(std::vector<Lexeme>& lexemes, int& i) {
    FunctionValue(lexemes, i);
    while (lexemes[i] == Type::Dot) {
        ++i;
        FunctionValue(lexemes, i);
    }
}

void Function(std::vector<Lexeme>& lexemes, int& i) {
    FunctionType(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw; // ќжидалось им€ функции
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw; // ќжиждалась кругла€ скобка
    ++i;
    if (lexemes[i] != Type::RightRoundBracket) {
        FucntionParameters(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket) {
        throw; // ќжидалась закрывающа€ кругла€ скобка
    }
    ++i;
    if (lexemes[i] != Type::LeftBrace) {
        throw; // ќжидалась фигурна€ скобка дл€ блока
    }
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace) {
        throw; // ќжидалась закрывающа€ фигурна€ скобка
    }
    ++i;
    return;
}

void SyntaciticAlalysis(std::vector<Lexeme>& lexemes, int& i) {
    while (i != lexemes.size()) {
        Function(lexemes, i);
    }
}