#include "SyntacticAnalysis.h"

void Expression(std::vector<Lexeme>& lexemes, int& i);
void Variable(std::vector<Lexeme>& lexemes, int& i);
void Expression0(std::vector<Lexeme>& lexemes, int& i);
void Expression1(std::vector<Lexeme>& lexemes, int& i);
void Expression2(std::vector<Lexeme>& lexemes, int& i);
void Expression3(std::vector<Lexeme>& lexemes, int& i);
void Expression4(std::vector<Lexeme>& lexemes, int& i);
void Expression5(std::vector<Lexeme>& lexemes, int& i);
void Expression6(std::vector<Lexeme>& lexemes, int& i);
void Equal(std::vector<Lexeme>& lexemes, int& i);
void Block(std::vector<Lexeme>& lexemes, int& i);
void type(std::vector<Lexeme>& lexemes, int& i);

void BaseType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "int"
        && lexemes[i].value_ != "float"
        && lexemes[i].value_ != "bool") 
        throw new ExceptionType(&lexemes[i]);
    ++i;
}

void ArrayType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "array") 
        throw new ExceptionArray(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftAngleBracket)
        throw new ExceptionLeftAngleBracket(&lexemes[i]);
    ++i;
    type(lexemes, i);
    if (lexemes[i] != Type::RightAngleBracket)
        throw new ExceptionRightAngleBracket(&lexemes[i]);
    ++i;
}

void type(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "array")
        ArrayType(lexemes, i);
    else
        BaseType(lexemes, i);
}

void CallFunction(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionFunctionName(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] == Type::RightRoundBracket) {
        ++i;
        return;
    }

    Expression(lexemes, i);
    while (lexemes[i] == Type::Comma) {
        ++i;
        Expression(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void ArrayIndexes(std::vector<Lexeme>& lexemes, int& i) {
    while (lexemes[i] == Type::LeftSquareBracket) {
        ++i;
        Expression(lexemes, i);
        if (lexemes[i] != Type::RightSquareBracket)
            throw new ExceptionRightSquareBracket(&lexemes[i]);
        ++i;
    }
}

void FunctionResult(std::vector<Lexeme>& lexemes, int& i) {
    CallFunction(lexemes, i);
    if (lexemes[i] == Type::LeftSquareBracket)
        ArrayIndexes(lexemes, i);
}

void Variable(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::LeftSquareBracket) {
        ArrayIndexes(lexemes, i);
    }
}

void Len(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "len")
        throw new ExceptionSpecial("len", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::RightRoundBracket) {
        ArrayIndexes(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Expression0(std::vector<Lexeme>& lexemes, int& i) {
    Expression1(lexemes, i);
    while (lexemes[i].value_ == "or") {
        ++i;
        Expression1(lexemes, i);
    }
}
void Expression1(std::vector<Lexeme>& lexemes, int& i) {
    Expression2(lexemes, i);
    if (lexemes[i] == Type::LeftAngleBracket || lexemes[i] == Type::RightAngleBracket
        || lexemes[i].value_ == "!=" || lexemes[i].value_ == "=="
        || lexemes[i].value_ == ">=" || lexemes[i].value_ == "<=") {
        ++i;
        Expression2(lexemes, i);
    }
}
void Expression2(std::vector<Lexeme>& lexemes, int& i) {
    Expression3(lexemes, i);
    while (lexemes[i].value_ == "&" || lexemes[i].value_ == "|" || lexemes[i].value_ == "^") {
        ++i;
        Expression3(lexemes, i);
    }
}
void Expression3(std::vector<Lexeme>& lexemes, int& i) {
    Expression4(lexemes, i);
    while (lexemes[i].value_ == "+" || lexemes[i].value_ == "-") {
        ++i;
        Expression4(lexemes, i);
    }
}
void Expression4(std::vector<Lexeme>& lexemes, int& i) {
    Expression5(lexemes, i);
    while (lexemes[i].value_ == "*" || lexemes[i].value_ == "/" || lexemes[i].value_ == "%") {
        ++i;
        Expression5(lexemes, i);
    }
}
void Expression5(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "-" || lexemes[i].value_ == "!" || lexemes[i].value_ == "~") {
        ++i;
    }
    Expression6(lexemes, i);
}
void Expression6(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] == Type::LeftRoundBracket) {
        ++i;
        Expression(lexemes, i);
        if (lexemes[i] != Type::RightRoundBracket) {
            throw new ExceptionRightRoundBracket(&lexemes[i]);
        }
        ++i;
        return;
    }
    if (lexemes[i] == Type::Int || lexemes[i] == Type::Float || lexemes[i] == Type::Bool) {
        ++i;
        return;
    }
    if (lexemes[i].value_ == "len") {
        Len(lexemes, i);
        return;
    }
    if (lexemes[i] == Type::Ident) {
        if (lexemes[i + 1] == Type::LeftRoundBracket) {
            FunctionResult(lexemes, i);
            return;
        } else {
            Variable(lexemes, i);
            return;
        }
    }
    throw new Exception("Неправильное выражение", &lexemes[i]);
}

void Expression(std::vector<Lexeme>& lexemes, int& i) {
    Expression0(lexemes, i);
    while (lexemes[i].value_ == "and") {
        ++i;
        Expression0(lexemes, i);
    }
}

void Equal(std::vector<Lexeme>& lexemes, int& i) {
    Variable(lexemes, i);
    if (lexemes[i] != Type::Equal)
        throw new ExceptionSpecial("=", &lexemes[i]);
    ++i;
    Expression(lexemes, i);
}

void VariableInit(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::Equal) {
        ++i;
        Expression(lexemes, i);
    }
}

void ArrayInit(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    ++i;
    ArrayIndexes(lexemes, i);
}

void NewVariable(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "array") {
        ArrayType(lexemes, i);
        ArrayInit(lexemes, i);
        while (lexemes[i] == Type::Comma) {
            ++i;
            ArrayInit(lexemes, i);
        }
    } else {
        BaseType(lexemes, i);
        VariableInit(lexemes, i);
        while (lexemes[i] == Type::Comma) {
            ++i;
            VariableInit(lexemes, i);
        }
    }
}

void If(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "if")
        throw new ExceptionSpecial("if", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    Expression(lexemes, i);
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::Special && lexemes[i].value_ == "else") {
        ++i;
        if (lexemes[i] != Type::LeftBrace)
            throw new ExceptionLeftBrace(&lexemes[i]);
        ++i;
        Block(lexemes, i);
        if (lexemes[i] != Type::RightBrace)
            throw new ExceptionRightBrace(&lexemes[i]);
        ++i;
    }
}

void While(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "while")
        throw new ExceptionSpecial("while", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    Expression(lexemes, i);
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
}

void ForVariableInit(std::vector<Lexeme>& lexemes, int& i) {
    NewVariable(lexemes, i);
    while (lexemes[i] == Type::Dot) {
        ++i;
        NewVariable(lexemes, i);
    }
}

void For(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "for")
        throw new ExceptionSpecial("for", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::Semicolon) {
        ForVariableInit(lexemes, i);
    }
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;

    Expression(lexemes, i);
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::RightRoundBracket) {
        Equal(lexemes, i);
        while (lexemes[i] == Type::Semicolon) {
            ++i;
            Equal(lexemes, i);
        }
    }

    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
}

void Input(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "input")
        throw new ExceptionSpecial("input", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    Variable(lexemes, i);
    while (lexemes[i] == Type::Comma) {
        ++i;
        Variable(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Print(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "print")
        throw new ExceptionSpecial("print", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    Expression(lexemes, i);
    while (lexemes[i] == Type::Comma) {
        ++i;
        Expression(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Return(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "return")
        throw new ExceptionSpecial("return", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::RightRoundBracket) {
        ++i;
        return;
    }
    Expression(lexemes, i);
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Action(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] == Type::Special) {
        if (lexemes[i].value_ == "if") {
            If(lexemes, i);
            return;
        }
        if (lexemes[i].value_ == "while") {
            While(lexemes, i);
            return;
        }
        if (lexemes[i].value_ == "for") {
            For(lexemes, i);
            return;
        }
        if (lexemes[i].value_ == "input") {
            Input(lexemes, i);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "print") {
            Print(lexemes, i);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "return") {
            Return(lexemes, i);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        NewVariable(lexemes, i);
        if (lexemes[i] != Type::Semicolon)
            throw new ExceptionSemicolon(&lexemes[i]);
        ++i;
        return;
    }
    
    if (lexemes[i] == Type::Ident) {
        int protected_i = i;
        Variable(lexemes, i);
        if (lexemes[i] == Type::Equal) {
            i = protected_i;
            Equal(lexemes, i);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        } else {
            i = protected_i;
            CallFunction(lexemes, i);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        throw new Exception(std::string("Ожидалось присваивание или вызов функции"), &lexemes[i]);
    }
    throw new Exception(std::string("Ожидалось какое-то действие"), &lexemes[i]);
}

void Block(std::vector<Lexeme>& lexemes, int& i) {
    while (lexemes[i] != Type::RightBrace) {
        Action(lexemes, i);
    }
}

void FunctionType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "void") {
        ++i;
        return;
    }
    type(lexemes, i);
}

void FunctionValue(std::vector<Lexeme>& lexemes, int& i) {
    type(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    ++i;
    while (lexemes[i] == Type::Comma) {
        ++i;
        if (lexemes[i] != Type::Ident) {
            throw new ExceptionVariableName(&lexemes[i]);
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
        throw new ExceptionFunctionName(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket) {
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    }
    ++i;
    if (lexemes[i] != Type::RightRoundBracket) {
        FucntionParameters(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket) {
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    }
    ++i;
    if (lexemes[i] != Type::LeftBrace) {
        throw new ExceptionLeftBrace(&lexemes[i]);
    }
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace) {
        throw new ExceptionRightBrace(&lexemes[i]);
    }
    ++i;
    return;
}

void SyntaciticAlalysis(std::vector<Lexeme>& lexemes) {
    int i = 0;
    while (i != lexemes.size()) {
        Function(lexemes, i);
    }
}