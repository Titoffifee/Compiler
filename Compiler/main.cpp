#include "SyntacticAnalysis.h"

int main() {
    try {
        std::vector<Lexeme> lexemes = LexicalAnalysis();
        SyntaciticAlalysis(lexemes);
    }
    catch (...) {

    }
    return 0;
}