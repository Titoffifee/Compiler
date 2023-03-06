#include "Exceptions.h"

ExceptionInRow::ExceptionInRow(std::string text, Lexeme* lexeme): 
    text_(text), lexeme_(lexeme) {}
void ExceptionInRow::print() {
    std::cout << "Ошибка в строке " << lexeme_->line_ << ":\n";
    std::cout << text_;
}

ExceptionInLexeme::ExceptionInLexeme(std::string text, Lexeme* lexeme) :
    ExceptionInRow(text, lexeme) {}
void ExceptionInLexeme::print() {
    std::cout << "Ошибка в лексеме в строке " << lexeme_->line_ << ":\n";
    std::cout << text_ << " вместо " << lexeme_->value_;
}