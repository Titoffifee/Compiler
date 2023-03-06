#include "Exceptions.h"

Exception::Exception(std::string text, Lexeme* lexeme):
    text_(text), lexeme_(lexeme) {}
void Exception::print() {
    std::cout << "������ � ������ " << lexeme_->line_ << ":\n";
    std::cout << text_;
}

ExceptionFunctionName::ExceptionFunctionName(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionFunctionName::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� ��� ������������ ������� ������ " << lexeme_->value_;
}

ExceptionLeftRoundBracket::ExceptionLeftRoundBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionLeftRoundBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '(' ������ " << lexeme_->value_;
}

ExceptionLeftBrace::ExceptionLeftBrace(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionLeftBrace::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '{' ������ " << lexeme_->value_;
}

ExceptionLeftSquareBracket::ExceptionLeftSquareBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionLeftSquareBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '[' ������ " << lexeme_->value_;
}

ExceptionLeftAngleBracket::ExceptionLeftAngleBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionLeftAngleBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '<' ������ " << lexeme_->value_;
}

ExceptionRightRoundBracket::ExceptionRightRoundBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionRightRoundBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� ')' ������ " << lexeme_->value_;
}

ExceptionRightBrace::ExceptionRightBrace(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionRightBrace::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '}' ������ " << lexeme_->value_;
}

ExceptionRightSquareBracket::ExceptionRightSquareBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionRightSquareBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� ']' ������ " << lexeme_->value_;
}

ExceptionRightAngleBracket::ExceptionRightAngleBracket(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionRightAngleBracket::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� '>' ������ " << lexeme_->value_;
}

ExceptionVariableName::ExceptionVariableName(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionVariableName::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� ��� ������������ ���������� ������ " << lexeme_->value_;
}

ExceptionSemicolon::ExceptionSemicolon(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionSemicolon::print() {
    std::cout << "������ � ������ " << lexeme_->line_ << ":\n";
    std::cout << "�� ������� ';'";
}

ExceptionSpecial::ExceptionSpecial(std::string word, Lexeme* lexeme) :
    Exception(word, lexeme) {}
void ExceptionSpecial::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "��������� ������� '" << text_ << "' ������ " << lexeme_->value_;
}

ExceptionType::ExceptionType(Lexeme* lexeme) :
    Exception("", lexeme) {}
void ExceptionType::print() {
    std::cout << "������ � ������� � ������ " << lexeme_->line_ << ":\n";
    std::cout << "�������� ��� ���������� ������ " << lexeme_->value_;
}