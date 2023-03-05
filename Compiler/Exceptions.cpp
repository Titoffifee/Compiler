#include "Exceptions.h"

ExceptionInRow::ExceptionInRow(std::string text, Lexeme* lexeme): 
    text_(text), lexeme_(lexeme) {}
void ExceptionInRow::print() {
    std::cout << "������ � ������ " << lexeme_->line_ << ":\n";
    std::cout << text_;
}

ExceptionInLexeme::ExceptionInLexeme(std::string text, Lexeme* lexeme) :
    ExceptionInRow(text, lexeme) {}
void ExceptionInLexeme::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << text_ << " ������ " << lexeme_->value_;
}