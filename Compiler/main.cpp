#include "SyntacticAnalysis.h"

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        std::vector<Lexeme> lexemes = LexicalAnalysis();
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