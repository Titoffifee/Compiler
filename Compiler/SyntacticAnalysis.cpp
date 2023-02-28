#include "SyntacticAnalysis.h"

void BaseType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Special) {
        throw; // �������� ���
    }
    if (lexemes[i].value_ == "int" || lexemes[i].value_ == "bool" || lexemes[i].value_ == "float") {
        ++i;
        return;
    }
    if (lexemes[i].value_ == "array") {
        ++i;
        if (lexemes[i] == Type::LeftAngleBracket) {
            ++i;
            BaseType(lexemes, i);
            if (lexemes[i] == Type::RightAngleBracket) {
                ++i;
                return;
            }
            throw; // ��������� ����������� ������� ������
        } 
        throw; // ��������� ����������� ������� ������
    }
    throw; // �������� ���
}

void Expression(std::vector<Lexeme>& lexemes, int& i);

void Equal(std::vector<Lexeme>& lexemes, int& i);

void VariableInit(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i] != Type::Ident) 
        throw; // ��������� ���
    ++i;
    if (lexemes[i] == Type::Equal) {
        ++i;
        Expression(lexemes, i);
    }
}

void NewVariable(std::vector<Lexeme>& lexemes, int& i) {
    BaseType(lexemes, i);
    VariableInit(lexemes, i);
    while (lexemes[i] == Type::Comma) {
        ++i;
        VariableInit(lexemes, i);
    }
}

void If(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "if")
        throw; // ��������� ����� "if"
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw; // ��������� ����������� ������� 
    ++i;
    Expression(lexemes, i);
    if (lexemes[i] != Type::RightRoundBracket)
        throw; // ��������� ����������� ������� 
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw; // ��������� ����������� �������� 
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw; // ��������� ����������� ��������
    ++i;
    if (lexemes[i] == Type::Special && lexemes[i].value_ == "else") {
        ++i;
        if (lexemes[i] != Type::LeftBrace)
            throw; // ��������� ����������� �������� 
        ++i;
        Block(lexemes, i);
        if (lexemes[i] != Type::RightBrace)
            throw; // ��������� ����������� ��������
        ++i;
    }
}

void While(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "while")
        throw; // ��������� ����� "while"
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw; // ��������� ����������� ������� 
    ++i;
    Expression(lexemes, i);
    if (lexemes[i] != Type::RightRoundBracket)
        throw; // ��������� ����������� ������� 
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw; // ��������� ����������� �������� 
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw; // ��������� ����������� ��������
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
        throw; // ��������� ����� "for"
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw; // ��������� ����������� ������� 
    ++i;

    if (lexemes[i] != Type::Semicolon) {
        ForVariableInit(lexemes, i);
    }
    if (lexemes[i] != Type::Semicolon)
        throw; // ����� ����� � �������
    ++i;

    Expression(lexemes, i);
    if (lexemes[i] != Type::Semicolon)
        throw; // ����� ����� � �������
    ++i;

    if (lexemes[i] != Type::RightRoundBracket) {
        Equal(lexemes, i);
        while (lexemes[i] == Type::Semicolon) {
            ++i;
            Equal(lexemes, i);
        }
    }

    if (lexemes[i] != Type::RightRoundBracket)
        throw; // ��������� ����������� �������
    ++i;

    if (lexemes[i] != Type::LeftBrace)
        throw; // ��������� ����������� �������� 
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace)
        throw; // ��������� ����������� ��������
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
        throw; //������������ ���� �����
    }
    
    if (lexemes[i] == Type::Ident) {

    }

    NewVariable(lexemes, i);
}

void Block(std::vector<Lexeme>& lexemes, int& i) {
    while (lexemes[i] != Type::RightBrace) {
        Block(lexemes, i);
        if (lexemes[i] != Type::Semicolon)
            throw; // �� ������� ����� � �������
        ++i;
    }
    ++i;
}

void FunctionType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "void") {
        ++i;
        return;
    }
    BaseType(lexemes, i);
}

void FunctionValue(std::vector<Lexeme>& lexemes, int& i) {
    BaseType(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw; // ��������� ��� ����������
    ++i;
    while (lexemes[i] == Type::Comma) {
        ++i;
        if (lexemes[i] != Type::Ident) {
            throw; // ��������� ��� ������������� ����������
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
        throw; // ��������� ��� �������
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket) {
        throw; // ���������� ������� ������
    }
    ++i;
    if (lexemes[i] != Type::RightRoundBracket) {
        FucntionParameters(lexemes, i);
    }
    if (lexemes[i] != Type::RightRoundBracket) {
        throw; // ��������� ����������� ������� ������
    }
    ++i;
    if (lexemes[i] != Type::LeftBrace) {
        throw; // ��������� �������� ������ ��� �����
    }
    ++i;
    Block(lexemes, i);
    if (lexemes[i] != Type::RightBrace) {
        throw; // ��������� ����������� �������� ������
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