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
        std::cout << "Всё корректно!";
    }
    catch (Exception* exception) {
        exception->print();
        delete exception;
    }
    catch (...) {
        std::cout << "Программа не дописана";
    }
    return 0;
}