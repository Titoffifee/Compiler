#include "SyntacticAnalysis.h"

int main() {
    try {
        std::vector<Lexeme> lexemes = LexicalAnalysis();
        for (auto lexeme : lexemes) {
            std::cout << lexeme << '\n';
        }
    }
    catch (...) {

    }
    return 0;
}