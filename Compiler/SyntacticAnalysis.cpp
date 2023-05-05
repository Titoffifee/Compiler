#include "SyntacticAnalysis.h"

VariableType* Expression(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Variable(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression0(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression1(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression2(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression3(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression4(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression5(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* Expression6(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
void Equal(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
void Block(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz);
VariableType* type(std::vector<Lexeme>& lexemes, int& i);

std::string ToString(int num) {
    if (num == 0) return "0";
    bool positive = bool(num > 0);
    num = abs(num);
    std::string result = "";
    while (num) {
        result += '0' + num % 10;
        num /= 10;
    }
    if (!positive) result += '-';
    std::reverse(result.begin(), result.end());
    return result;
}

VariableType* BaseType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "int"
        && lexemes[i].value_ != "float"
        && lexemes[i].value_ != "bool") 
        throw new ExceptionType(&lexemes[i]);
    VariableType* t = new VariableType(lexemes[i]);
    ++i;
    return t;
}

VariableType* ArrayType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ != "array") 
        throw new ExceptionArray(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftAngleBracket)
        throw new ExceptionLeftAngleBracket(&lexemes[i]);
    ++i;
    VariableType* t = new VariableType(type(lexemes, i));
    if (lexemes[i] != Type::RightAngleBracket)
        throw new ExceptionRightAngleBracket(&lexemes[i]);
    ++i;
    return t;
}

VariableType* type(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "array")
        return ArrayType(lexemes, i);
    else
        return BaseType(lexemes, i);
}

VariableType* CallFunction(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionFunctionName(&lexemes[i]);
    Lexeme* function_name = &lexemes[i];
    VariableType* return_value = function_name_space->GetReturnValue(function_name);
    FunctionParameter* function_parameters = function_name_space->GetFunctionParametrs(function_name);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] == Type::RightRoundBracket) {
        if (function_parameters != nullptr)
            throw new ExceptionFunctionUndeclared(function_name);
        ++i;
        poliz.push_back(function_name->value_);
        return return_value;
    }

    FunctionParameter* params = new FunctionParameter(Expression(lexemes, i, name_space, function_name_space, poliz));
    while (lexemes[i] == Type::Comma) {
        ++i;
        params->AddToEnd(Expression(lexemes, i, name_space, function_name_space, poliz));
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (!TwoParamsEqual(function_parameters, params))
        throw new ExceptionFunctionUndeclared(function_name);
    delete params;
    poliz.push_back(function_name->value_);
    return (return_value == nullptr ? nullptr : return_value->GetFullCopy());
}

VariableType* ArrayIndexes(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* array_type, std::vector <std::string>& poliz, int& cnt) {
    std::vector <std::vector <std::string> > crt;
    while (lexemes[i] == Type::LeftSquareBracket) {
        ++i;
        crt.resize(crt.size() + 1);
        if (!Expression(lexemes, i, name_space, function_name_space, crt.back())->IsBasicType())
            throw new ExceptionWrongExpressionResult(lexemes[i].line_);
        if (lexemes[i] != Type::RightSquareBracket)
            throw new ExceptionRightSquareBracket(&lexemes[i]);
        array_type = array_type->Next();
        if (array_type == nullptr)
            throw new ExceptionArrayEnd(&lexemes[i]);
        ++i;
    }
    if (cnt != -1) {
        cnt = crt.size();
        std::reverse(crt.begin(), crt.end());
        for (auto& elem : crt) {
            for (auto& i : elem) {
                poliz.push_back(i);
            }
        }
    }
    return array_type->GetFullCopy();
}

VariableType* FunctionResult(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    std::vector <std::string> crt;
    VariableType* result = CallFunction(lexemes, i, name_space, function_name_space, crt);
    if (lexemes[i] == Type::LeftSquareBracket) {
        int cnt = 0;
        VariableType* correct_result = ArrayIndexes(lexemes, i, name_space, function_name_space, result, poliz, cnt);
        delete result;
        result = correct_result;
        for (auto& i : crt) {
            poliz.push_back(i);
        }
        while (cnt--) poliz.push_back("[]");
    }
    else {
        for (auto& i : crt) {
            poliz.push_back(i);
        }
    }
    return result;
}

VariableType* Variable(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    VariableType* variable_type = GetVariableType(lexemes[i], name_space);
    ++i;
    if (lexemes[i] == Type::LeftSquareBracket) {
        std::string name = lexemes[i - 1].value_;
        int cnt = 0;
        VariableType* correct_variable_type = ArrayIndexes(lexemes, i, name_space, function_name_space, variable_type, poliz, cnt);
        delete variable_type;
        poliz.push_back(name);
        while (cnt--) poliz.push_back("[]");
        return correct_variable_type;
    }
    else {
        poliz.push_back(lexemes[i - 1].value_);
    }
    return variable_type;
}

/*void Len(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "len")
        throw new ExceptionSpecial("len", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    VariableType* array_type = GetVariableType(lexemes[i], name_space);
    ++i;
    if (lexemes[i] != Type::RightRoundBracket) {
        VariableType* correct_type = ArrayIndexes(lexemes, i, name_space, function_name_space, array_type);
        if (!correct_type->IsArray())
            throw new ExceptionWaitingArray(&lexemes[i]);
        delete correct_type;
    } else {
        if (!array_type->IsArray())
            throw new ExceptionWaitingArray(&lexemes[i]);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}*/

VariableType* Expression0(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression1(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i].value_ == "or") {
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression1(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back("or");
    }
    return expression_type;
}
VariableType* Expression1(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression2(lexemes, i, name_space, function_name_space, poliz);
    if (lexemes[i] == Type::LeftAngleBracket || lexemes[i] == Type::RightAngleBracket
        || lexemes[i].value_ == "!=" || lexemes[i].value_ == "=="
        || lexemes[i].value_ == ">=" || lexemes[i].value_ == "<=") {
        std::string oper = lexemes[i].value_;
        ++i;
        if (!expression_type->IsBasicType())
            throw new ExceptionWrongExpressionType(lexemes[i].line_);
        expression_type = CheckBinExpression(expression_type,
            Expression2(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back(oper);

    }
    return expression_type;
}
VariableType* Expression2(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression3(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i].value_ == "&" || lexemes[i].value_ == "|" || lexemes[i].value_ == "^") {
        std::string oper = lexemes[i].value_;
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression3(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back(oper);
    }

    return expression_type;
}
VariableType* Expression3(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression4(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i].value_ == "+" || lexemes[i].value_ == "-") {
        std::string oper = lexemes[i].value_;
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression4(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back(oper);
    }
    return expression_type;
}
VariableType* Expression4(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression5(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i].value_ == "*" || lexemes[i].value_ == "/" || lexemes[i].value_ == "%") {
        std::string oper = lexemes[i].value_;
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression5(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back(oper);
    }
    return expression_type;
}
VariableType* Expression5(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "-" && lexemes[i].value_ != "!" && lexemes[i].value_ != "~") {
        return Expression6(lexemes, i, name_space, function_name_space, poliz);
    }
    std::string oper = lexemes[i].value_;
    if (oper == "-") poliz.push_back("0");
    ++i;
    auto res = CheckUnoExpression(Expression6(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    poliz.push_back(oper);
    return res;
}
VariableType* Expression6(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i] == Type::LeftRoundBracket) {
        ++i;
        VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space, poliz);
        if (lexemes[i] != Type::RightRoundBracket) {
            throw new ExceptionRightRoundBracket(&lexemes[i]);
        }
        ++i;
        return expression_type;
    }
    if (lexemes[i] == Type::Int || lexemes[i] == Type::Float || lexemes[i] == Type::Bool) {
        poliz.push_back(lexemes[i].value_);
        ++i;
        return new VariableType(lexemes[i-1].type_);
    }
    if (lexemes[i] == Type::Ident) {
        if (lexemes[i + 1] == Type::LeftRoundBracket) {
            return FunctionResult(lexemes, i, name_space, function_name_space, poliz);
        } else {
            return Variable(lexemes, i, name_space, function_name_space, poliz);
        }
    }
    throw new Exception("Неправильное выражение", &lexemes[i]);
}
VariableType* Expression(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* expression_type = Expression0(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i].value_ == "and") {
        ++i;
        expression_type = CheckBinExpression(expression_type, Expression0(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back("and");
    }
    return expression_type;
}

void Equal(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* variable_type = Variable(lexemes, i, name_space, function_name_space, poliz);
    if (lexemes[i] != Type::Equal)
        throw new ExceptionSpecial("=", &lexemes[i]);
    std::string oper = lexemes[i].value_;
    ++i;
    CheckCanDoEqual(variable_type, Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    delete variable_type;
    poliz.push_back(oper);
}

void VariableInit(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* variable_type, std::vector <std::string>& poliz) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], variable_type);
    std::string name = lexemes[i].value_;
    ++i;
    if (lexemes[i] == Type::Equal) {
        ++i;
        poliz.push_back(name);
        CheckCanDoEqual(variable_type, Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back("=");
    }
}

void ArrayInit(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* array_type, std::vector <std::string>& poliz) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], array_type);
    std::string name = lexemes[i].value_;
    ++i;
    int cnt = -1;
    VariableType* final_cell = ArrayIndexes(lexemes, i, name_space, function_name_space, array_type, poliz, cnt);
    if (!final_cell->IsBasicType())
        throw new ExceptionArrayInitFinalCellNotBasic(lexemes[i].line_);
    delete final_cell;
    if (lexemes[i] == Type::Equal) {
        ++i;
        poliz.push_back(name);
        CheckCanDoEqual(array_type, Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
        poliz.push_back("=");
    }
}

void NewVariable(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* variable_type = nullptr;
    if (lexemes[i].value_ == "array") {
        variable_type = ArrayType(lexemes, i);
        ArrayInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy(), poliz);
        while (lexemes[i] == Type::Comma) {
            ++i;
            ArrayInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy(), poliz);
        }
    } else {
        variable_type = BaseType(lexemes, i);
        VariableInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy(), poliz);
        while (lexemes[i] == Type::Comma) {
            ++i;
            VariableInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy(), poliz);
        }
    }
    delete variable_type;
}

void If(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "if")
        throw new ExceptionSpecial("if", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space, poliz);
    int p1 = poliz.size();
    poliz.push_back("0");
    poliz.push_back("F!");
    if (!expression_type->IsBasicType())
        throw new ExceptionWrongExpressionResult(lexemes[i].line_);
    delete expression_type;
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space, poliz);
    poliz[p1] = ToString(poliz.size() - p1);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::Special && lexemes[i].value_ == "else") {
        poliz[p1] = ToString(poliz.size() - p1 + 2);
        int p2 = poliz.size();
        poliz.push_back("0");
        poliz.push_back("!");
        ++i;
        if (lexemes[i] != Type::LeftBrace)
            throw new ExceptionLeftBrace(&lexemes[i]);
        ++i;
        Block(lexemes, i, name_space, function_name_space, poliz);
        if (lexemes[i] != Type::RightBrace)
            throw new ExceptionRightBrace(&lexemes[i]);
        ++i;
        poliz[p2] = ToString(poliz.size() - p2);
    }
}

void While(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "while")
        throw new ExceptionSpecial("while", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    int p1 = poliz.size();
    VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space, poliz);
    int p2 = poliz.size();
    poliz.push_back("0");
    poliz.push_back("F!");
    if (!expression_type->IsBasicType())
        throw new ExceptionWrongExpressionResult(lexemes[i].line_);
    delete expression_type;
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space, poliz);
    poliz.push_back(ToString(p1 - poliz.size()));
    poliz.push_back("!");
    poliz[p2] = ToString(poliz.size() - p2);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
}

void ForVariableInit(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    NewVariable(lexemes, i, name_space, function_name_space, poliz);
    while (lexemes[i] == Type::Dot) {
        ++i;
        NewVariable(lexemes, i, name_space, function_name_space, poliz);
    }
}

void For(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    name_space = new NameSpace(name_space);
    if (lexemes[i].value_ != "for")
        throw new ExceptionSpecial("for", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::Semicolon) {
        ForVariableInit(lexemes, i, name_space, function_name_space, poliz);
    }
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;
    int p1 = poliz.size();
    CheckIsResultBasicAndDelete(Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;
    int p2 = poliz.size();
    poliz.push_back("0");
    poliz.push_back("F!");
    int p3 = poliz.size();
    poliz.push_back("0");
    poliz.push_back("!");
    if (lexemes[i] != Type::RightRoundBracket) {
        Equal(lexemes, i, name_space, function_name_space, poliz);
        while (lexemes[i] == Type::Semicolon) {
            ++i;
            Equal(lexemes, i, name_space, function_name_space, poliz);
        }
    }
    int p4 = poliz.size();
    poliz.push_back(ToString(p1 - poliz.size()));
    poliz.push_back("!");
    poliz[p3] = ToString(poliz.size() - p3);
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space, poliz);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
    poliz.push_back(ToString(p3 + 2 - poliz.size()));
    poliz.push_back("!");
    poliz[p2] = ToString(poliz.size() - p2);
    delete name_space;
}

void Input(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "input")
        throw new ExceptionSpecial("input", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    int cnt = 1;
    CheckIsResultBasicAndDelete(Variable(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    while (lexemes[i] == Type::Comma) {
        ++i;
        ++cnt;
        CheckIsResultBasicAndDelete(Variable(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    }
    poliz.push_back(ToString(cnt));
    poliz.push_back("input");
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Print(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "print")
        throw new ExceptionSpecial("print", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    CheckIsResultBasicAndDelete(Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    int cnt = 1;
    while (lexemes[i] == Type::Comma) {
        ++i;
        ++cnt;
        CheckIsResultBasicAndDelete(Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    }
    poliz.push_back(ToString(cnt));
    poliz.push_back("print");
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

VariableType* current_return_value = nullptr;

void Return(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i].value_ != "return")
        throw new ExceptionSpecial("return", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::RightRoundBracket) {
        ++i;
        if(current_return_value == nullptr)
            return;
        throw new ExceptionWrongExpressionResult(lexemes[i].line_);
    }
    CheckCanDoEqual(current_return_value, Expression(lexemes, i, name_space, function_name_space, poliz), lexemes[i].line_);
    poliz.push_back("return");
    poliz.push_back("end");
    poliz.push_back("!");
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Action(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    if (lexemes[i] == Type::Special) {
        if (lexemes[i].value_ == "if") {
            If(lexemes, i, name_space, function_name_space, poliz);
            return;
        }
        if (lexemes[i].value_ == "while") {
            While(lexemes, i, name_space, function_name_space, poliz);
            return;
        }
        if (lexemes[i].value_ == "for") {
            For(lexemes, i, name_space, function_name_space, poliz);
            return;
        }
        if (lexemes[i].value_ == "input") {
            Input(lexemes, i, name_space, function_name_space, poliz);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "print") {
            Print(lexemes, i, name_space, function_name_space, poliz);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "return") {
            Return(lexemes, i, name_space, function_name_space, poliz);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        NewVariable(lexemes, i, name_space, function_name_space, poliz);
        if (lexemes[i] != Type::Semicolon)
            throw new ExceptionSemicolon(&lexemes[i]);
        ++i;
        return;
    }
    
    if (lexemes[i] == Type::Ident) {
        if (lexemes[i + 1] == Type::LeftRoundBracket) {
            CallFunction(lexemes, i, name_space, function_name_space, poliz);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
        } else {
            Equal(lexemes, i, name_space, function_name_space, poliz);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
        }
        return;
        // throw new Exception(std::string("Ожидалось присваивание или вызов функции"), &lexemes[i]);
    }
    throw new Exception(std::string("Ожидалось какое-то действие"), &lexemes[i]);
}

void Block(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    NameSpace* new_name_space = new NameSpace(name_space);
    while (lexemes[i] != Type::RightBrace) {
        Action(lexemes, i, new_name_space, function_name_space, poliz);
    }
    delete new_name_space;
}

VariableType* FunctionType(std::vector<Lexeme>& lexemes, int& i) {
    if (lexemes[i].value_ == "void") {
        ++i;
        return nullptr;
    }
    return type(lexemes, i);
}

FunctionParameter* FunctionValue(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, std::vector <std::string>& poliz) {
    VariableType* params_type = type(lexemes, i);
    FunctionParameter* params = new FunctionParameter(params_type->GetFullCopy());
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], params_type->GetFullCopy());
    poliz.push_back(lexemes[i].value_);
    ++i;
    while (lexemes[i] == Type::Comma) {
        ++i;
        params = new FunctionParameter(params_type->GetFullCopy(), params);
        if (lexemes[i] != Type::Ident) {
            throw new ExceptionVariableName(&lexemes[i]);
        }
        name_space->Add(lexemes[i], params_type);
        poliz.push_back(lexemes[i].value_);
        ++i;
    }
    delete params_type;
    return params;
}

FunctionParameter* FucntionParameters(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, std::vector <std::string>& poliz) {
    FunctionParameter* params = FunctionValue(lexemes, i, name_space, poliz);
    while (lexemes[i] == Type::Dot) {
        ++i;
        params->AddToEnd(FunctionValue(lexemes, i, name_space, poliz));
    }
    return params;
}

void Function(std::vector<Lexeme>& lexemes, int& i, 
    FunctionNameSpace* function_name_space, std::vector <std::string>& poliz) {
    VariableType* return_value = FunctionType(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw new ExceptionFunctionName(&lexemes[i]);
    Lexeme* function_name = &lexemes[i];
    poliz.push_back(function_name->value_ + ":");
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket) {
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    }
    ++i;
    NameSpace* name_space = new NameSpace();
    FunctionParameter* params = nullptr;
    if (lexemes[i] != Type::RightRoundBracket) {
        params = FucntionParameters(lexemes, i, name_space, poliz);
    }
    if (lexemes[i] != Type::RightRoundBracket) {
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    }
    ++i;
    if (lexemes[i] != Type::LeftBrace) {
        throw new ExceptionLeftBrace(&lexemes[i]);
    }
    ++i;
    current_return_value = return_value;
    function_name_space->AddFunction(function_name, return_value, params);
    Block(lexemes, i, name_space, function_name_space, poliz);
    if (lexemes[i] != Type::RightBrace) {
        throw new ExceptionRightBrace(&lexemes[i]);
    }
    ++i;
}

void SyntaciticAnalysis(std::vector<Lexeme>& lexemes, std::vector <std::vector <std::string>>& poliz) {
    int i = 0;
    FunctionNameSpace* function_name_space = new FunctionNameSpace();
    while (i != lexemes.size()) {
        poliz.resize(poliz.size() + 1);
        Function(lexemes, i, function_name_space, poliz.back());
    }
}