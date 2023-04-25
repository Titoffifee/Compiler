#include "ExceptionsSemantic.h"

ExceptionVariableRedeclaration::ExceptionVariableRedeclaration(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionVariableRedeclaration::print() {
    std::cout << "Переобъявление переменной " << '"' << lexeme_->value_ << '"' << " в строке " << lexeme_->line_;
}

ExceptionFunctionRedeclaration::ExceptionFunctionRedeclaration(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionFunctionRedeclaration::print() {
    std::cout << "Переобъявление функции " << '"' << lexeme_->value_ << '"' << " в строке " << lexeme_->line_;
}

ExceptionVariableUndeclared::ExceptionVariableUndeclared(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionVariableUndeclared::print() {
    std::cout << "Необъявленная переменная " << '"' << lexeme_->value_ << '"' << " в строке " << lexeme_->line_;
}

ExceptionFunctionUndeclared::ExceptionFunctionUndeclared(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionFunctionUndeclared::print() {
    std::cout << "Необъявленная функция " << '"' << lexeme_->value_ << '"' << " в строке " << lexeme_->line_;
}

ExceptionOperator::ExceptionOperator(bool turn, Lexeme* lexeme) :
    Exception((turn ? "справа" : "слева"), lexeme) {}
void ExceptionOperator::print_turn() {
    std::cout << ' ' << text_ << " от оператора " << lexeme_->value_;
}

ExceptionWrongExpressionType::ExceptionWrongExpressionType(int line) :
    Exception("", nullptr), line_(line) {}
void ExceptionWrongExpressionType::print() {
    std::cout << "Ошибка в строке " << line_ << ": " << "Неприводимый тип";
}