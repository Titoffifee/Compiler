#include "LexicalAnalysis.h"

Lexeme::Lexeme() : type(Type::Special), value("") {}
Lexeme::Lexeme(Type type_, std::string value_) :
    type(type_), value(value_) {}
std::ostream& operator << (std::ostream& out, Lexeme& lex) {
    out << "<" << int(lex.type) << ' ' << lex.value << ">";
    return out;
}
std::ofstream& operator << (std::ofstream& out, Lexeme& lex) {
    out << "<" << int(lex.type) << ' ' << lex.value << ">";
    return out;
}

void Comment(std::string& code, int& it, bool& correct) {
    ++it;
    while (it < code.size() && code[it] != '@') ++it;
    if (it == code.size()) correct = false;
}

bool IsSpecial(std::string& value, std::vector <std::string>& special) {
    for (auto str : special) {
        if (str == value) return true;
    }
    return false;
}

void String(std::string& code, int& it, std::vector <Lexeme>& lexemes, std::vector <std::string>& special) {
    std::string value = "";
    while (it < code.size() && (code[it] == '_' || 'a' <= code[it] && code[it] <= 'z' || 'A' <= code[it] && code[it] <= 'Z')) {
        value += code[it++];
    }
    --it;
    if (IsSpecial(value, special)) {
        lexemes.push_back(Lexeme(Type::Special, value));
    } else {
        lexemes.push_back(Lexeme(Type::Ident, value));
    }
}

void Number(std::string& code, int& it, std::vector <Lexeme>& lexemes) {
    std::string crt = "";
    if ((code[it + 1] < '0' || '9' < code[it + 1]) && code[it + 1] != '.') {
        crt += code[it];
        if (code[it] == '0' || code[it] == '1') {
            lexemes.push_back(Lexeme(Type::Bool, crt));
            return;
        }
        lexemes.push_back(Lexeme(Type::Int, crt));
        return;
    }
    while ('0' <= code[it] && code[it] <= '9') {
        crt += code[it++];
    }
    if (code[it] != '.') {
        lexemes.push_back(Lexeme(Type::Int, crt));
        --it;
        return;
    }
    ++it;
    crt += '.';
    while ('0' <= code[it] && code[it] <= '9') {
        crt += code[it++];
    }
    --it;
    lexemes.push_back(Lexeme(Type::Float, crt));
    return;
}

void Split(std::string& code, int it, std::vector <Lexeme>& lexemes, std::vector <std::string>& special) {
    while (it < code.size()) {
        switch (code[it]) {
        case ' ':
            ++it;
            break;
        case '(':
            lexemes.push_back(Lexeme(Type::LeftRoundBracket, "("));
            ++it;
            break;
        case ')':
            lexemes.push_back(Lexeme(Type::RightRoundBracket, ")"));
            ++it;
            break;
        case '[':
            lexemes.push_back(Lexeme(Type::LeftSquareBracket, "["));
            ++it;
            break;
        case ']':
            lexemes.push_back(Lexeme(Type::RightSquareBracket, "]"));
            ++it;
            break;
        case '{':
            lexemes.push_back(Lexeme(Type::LeftBrace, "{"));
            ++it;
            break;
        case '}':
            lexemes.push_back(Lexeme(Type::RightBrace, "}"));
            ++it;
            break;
        case '.':
            lexemes.push_back(Lexeme(Type::Dot, "."));
            ++it;
            break;
        case ',':
            lexemes.push_back(Lexeme(Type::Comma, ","));
            ++it;
            break;
        case '@':
            Comment(code, it, correct);
            ++it;
            break;
        case '/':
            lexemes.push_back(Lexeme(Type::Operation, "/"));
            ++it;
            break;
        case '*':
            lexemes.push_back(Lexeme(Type::Operation, "*"));
            ++it;
            break;
        case '+':
            lexemes.push_back(Lexeme(Type::Operation, "+"));
            ++it;
            break;
        case '-':
            lexemes.push_back(Lexeme(Type::Operation, "-"));
            ++it;
            break;
        case '=':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, "=="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Equal, "="));
            }
            break;
        case ';':
            lexemes.push_back(Lexeme(Type::Semicolon, ";"));
            ++it;
            break;
        case '!':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, "!="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, "!"));
            }
            break;
        case '<':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, "<="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, "<"));
            }
            break;
        case '>':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, ">="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, ">"));
            }
            break;
        default:
            if ('0' <= code[it] && code[it] <= '9') {
                Number(code, it, lexemes);
                ++it;
            } else if (code[it] == '_' || 'a' <= code[it] && code[it] <= 'z' || 'A' <= code[it] && code[it] <= 'Z') {
                String(code, it, lexemes, special);
                ++it;
            } else {
                std::cout << code[it] << ' ' << it << '\n';
                correct = false;
                return;
            }
        }
    }
}

std::vector<Lexeme> LexicalAnalysis() {
    std::string input = "";
    std::ifstream code("Data/Input.txt");
    std::ifstream spec("Data/SpecialWords.txt");
    // std::ofstream output("Data/Output.txt");
    std::string crt;
    while (code >> crt) {
        input += crt + ' ';
    }
    std::vector <std::string> special;
    while (spec >> crt) {
        special.push_back(crt);
    }
    // output << input + '\n';
    std::vector <Lexeme> lexemes;
    Split(input, 0, lexemes, special);
    code.close();
    return lexemes;
    /*if (!correct) {
        output << "It's incorrect code\n";
        for (auto lexeme : lexemes) {
            output << lexeme << '\n';
        }
    } else {
        for (auto lexeme : lexemes) {
            output << lexeme << '\n';
        }
    }
    output.close();*/
}