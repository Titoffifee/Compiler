#include "LexicalAnalysis.h"

Lexeme::Lexeme(Type type, int line, std::string value) :
    type_(type), line_(line), value_(value) {}

std::ostream& operator<< (std::ostream& out, Lexeme& lex) {
    out << "<" << int(lex.type_) << ' ' << lex.value_ << ' ' << lex.line_ << ">";
    return out;
}
bool Lexeme::operator==(Type type) {
    return type_ == type;
}
bool Lexeme::operator!=(Type type) {
    return type_ != type;
}

void Comment(std::string& code, int& it, int& crt) {
    ++it;
    while (it < code.size() && code[it] != '@') {
        if (code[it] == '\n') ++crt;
        ++it;
    }
    if (it == code.size())
        throw;
}

bool IsSpecial(std::string& value, std::vector <std::string>& special) {
    for (auto str : special) {
        if (str == value) return true;
    }
    return false;
}

void String(std::string& code, int& it, std::vector <Lexeme>& lexemes, std::vector <std::string>& special, int line) {
    std::string value = "";
    while (it < code.size() && (code[it] == '_' || 'a' <= code[it] && code[it] <= 'z' || 'A' <= code[it] && code[it] <= 'Z')) {
        value += code[it++];
    }
    --it;
    if (IsSpecial(value, special)) {
        lexemes.push_back(Lexeme(Type::Special, line, value));
    } else {
        lexemes.push_back(Lexeme(Type::Ident, line, value));
    }
}

void Number(std::string& code, int& it, std::vector <Lexeme>& lexemes, int line) {
    std::string crt = "";
    if ((code[it + 1] < '0' || '9' < code[it + 1]) && code[it + 1] != '.') {
        crt += code[it];
        if (code[it] == '0' || code[it] == '1') {
            lexemes.push_back(Lexeme(Type::Bool, line, crt));
            return;
        }
        lexemes.push_back(Lexeme(Type::Int, line ,crt));
        return;
    }
    while ('0' <= code[it] && code[it] <= '9') {
        crt += code[it++];
    }
    if (code[it] != '.') {
        lexemes.push_back(Lexeme(Type::Int, line, crt));
        --it;
        return;
    }
    ++it;
    crt += '.';
    while ('0' <= code[it] && code[it] <= '9') {
        crt += code[it++];
    }
    --it;
    lexemes.push_back(Lexeme(Type::Float, line, crt));
    return;
}

void Split(std::string& code, int it, std::vector <Lexeme>& lexemes, std::vector <std::string>& special) {
    int line = 0;
    while (it < code.size()) {
        switch (code[it]) {
        case '\n':
            ++it;
            ++line;
            break;
        case ' ':
            ++it;
            break;
        case '(':
            lexemes.push_back(Lexeme(Type::LeftRoundBracket, line, "("));
            ++it;
            break;
        case ')':
            lexemes.push_back(Lexeme(Type::RightRoundBracket, line, ")"));
            ++it;
            break;
        case '[':
            lexemes.push_back(Lexeme(Type::LeftSquareBracket, line, "["));
            ++it;
            break;
        case ']':
            lexemes.push_back(Lexeme(Type::RightSquareBracket, line, "]"));
            ++it;
            break;
        case '{':
            lexemes.push_back(Lexeme(Type::LeftBrace, line, "{"));
            ++it;
            break;
        case '}':
            lexemes.push_back(Lexeme(Type::RightBrace, line, "}"));
            ++it;
            break;
        case '.':
            lexemes.push_back(Lexeme(Type::Dot, line, "."));
            ++it;
            break;
        case ',':
            lexemes.push_back(Lexeme(Type::Comma, line, ","));
            ++it;
            break;
        case '@':
            Comment(code, it, line);
            ++it;
            break;
        case '/':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "/=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "/"));
            ++it;
            break;
        case '*':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "*=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "*"));
            ++it;
            break;
        case '+':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "+=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "+"));
            ++it;
            break;
        case '-':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "-=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "-"));
            ++it;
            break;
        case '%':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "%=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "%"));
            ++it;
            break;
        case '|':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "|=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "|"));
            ++it;
            break;
        case '&':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "&=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "&"));
            ++it;
            break;
        case '^':
            if (code[it + 1] == '=') lexemes.push_back(Lexeme(Type::Equal, line, "^=")), ++it;
            else lexemes.push_back(Lexeme(Type::Operation, line, "^"));
            ++it;
            break;
        case '=':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, line, "=="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Equal, line, "="));
            }
            break;
        case ';':
            lexemes.push_back(Lexeme(Type::Semicolon, line, ";"));
            ++it;
            break;
        case '!':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, line, "!="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, line, "!"));
            }
            break;
        case '<':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, line, "<="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, line, "<"));
            }
            break;
        case '>':
            if (code[it + 1] == '=') {
                it += 2;
                lexemes.push_back(Lexeme(Type::Operation, line, ">="));
            } else {
                ++it;
                lexemes.push_back(Lexeme(Type::Operation, line, ">"));
            }
            break;
        default:
            if ('0' <= code[it] && code[it] <= '9') {
                Number(code, it, lexemes, line);
                ++it;
            } else if (code[it] == '_' || 'a' <= code[it] && code[it] <= 'z' || 'A' <= code[it] && code[it] <= 'Z') {
                String(code, it, lexemes, special, line);
                ++it;
            } else {
                std::cout << code[it] << ' ' << it << '\n';
                throw "something";
            }
        }
    }
}

std::vector<Lexeme> LexicalAnalysis() {
    std::string input = "";
    std::ifstream code("Data/Input.txt");
    std::ifstream spec("Data/SpecialWords.txt");
    std::string crt;
    while (!code.eof()) {
        std::getline(code, crt);
        for (char c : crt) {
            if (c != '  ') input += c;
            else input += ' ';
        }
        input += '\n';
    }
    std::vector <std::string> special;
    while (spec >> crt) {
        special.push_back(crt);
    }
    std::vector <Lexeme> lexemes;
    Split(input, 0, lexemes, special);
    code.close();
    spec.close();
    return lexemes;
}