#include "ExceptionsSemantic.h"

ExceptionVariableRedeclaration::ExceptionVariableRedeclaration(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionVariableRedeclaration::print() {
    std::cout << "�������������� ���������� " << '"' << lexeme_->value_ << '"' << " � ������ " << lexeme_->line_;
}

ExceptionFunctionRedeclaration::ExceptionFunctionRedeclaration(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionFunctionRedeclaration::print() {
    std::cout << "�������������� ������� " << '"' << lexeme_->value_ << '"' << " � ������ " << lexeme_->line_;
}

ExceptionVariableUndeclared::ExceptionVariableUndeclared(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionVariableUndeclared::print() {
    std::cout << "������������� ���������� " << '"' << lexeme_->value_ << '"' << " � ������ " << lexeme_->line_;
}

ExceptionFunctionUndeclared::ExceptionFunctionUndeclared(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionFunctionUndeclared::print() {
    std::cout << "������������� ������� " << '"' << lexeme_->value_ << '"' << " � ������ " << lexeme_->line_;
}

ExceptionOperator::ExceptionOperator(bool turn, Lexeme* lexeme) :
    Exception((turn ? "������" : "�����"), lexeme) {}
void ExceptionOperator::print_turn() {
    std::cout << ' ' << text_ << " �� ��������� " << lexeme_->value_;
}

ExceptionWrongExpressionType::ExceptionWrongExpressionType(int line) :
    Exception("", nullptr), line_(line) {}
void ExceptionWrongExpressionType::print() {
    std::cout << "������ � ������ " << line_ << ": " << "������������ ���";
}