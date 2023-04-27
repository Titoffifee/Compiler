#include "SyntacticAnalysis.h"

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<Lexeme> lexemes;
    try {
        lexemes = LexicalAnalysis();
    }
    catch (std::string error) {
        std::cout << error;
        return 0;
    }
    try {
        SyntaciticAlalysis(lexemes);
        std::cout << "�� ���������!";
    }
    catch (Exception* exception) {
        exception->print();
        delete exception;
    }
    catch (...) {
        std::cout << "��������� �� ��������";
    }
    return 0;
}