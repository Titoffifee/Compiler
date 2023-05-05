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
    std::vector <std::vector <std::string>> poliz;
    try {
        SyntaciticAnalysis(lexemes, poliz);
        std::cout << "Всё корректно!";
        for (auto& function : poliz) {
            std::cout << '\n';
            for (auto elem : function) {
                std::cout << elem << ' ';
            }
        }
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