#include "SyntacticAnalysis.h"

VariableType* Expression(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Variable(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression0(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression1(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression2(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression3(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression4(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression5(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* Expression6(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
void Equal(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
void Block(std::vector<Lexeme>& lexemes, int& i, NameSpace* name_space, FunctionNameSpace* function_name_space);
VariableType* type(std::vector<Lexeme>& lexemes, int& i);

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
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
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
        return return_value;
    }

    FunctionParameter* params = new FunctionParameter(Expression(lexemes, i, name_space, function_name_space));
    while (lexemes[i] == Type::Comma) {
        ++i;
        params->AddToEnd(Expression(lexemes, i, name_space, function_name_space));
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (TwoParamsEqual(function_parameters, params))
        throw new ExceptionFunctionUndeclared(function_name);
    delete params;
    return return_value->GetFullCopy();
}

VariableType* ArrayIndexes(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* array_type) {
    while (lexemes[i] == Type::LeftSquareBracket) {
        ++i;
        if (!Expression(lexemes, i, name_space, function_name_space)->IsBaseType())
            throw new ExceptionWrongExpressionResult(lexemes[i].line_);
        if (lexemes[i] != Type::RightSquareBracket)
            throw new ExceptionRightSquareBracket(&lexemes[i]);
        array_type = array_type->Next();
        if (array_type == nullptr)
            throw new ExceptionArrayEnd(&lexemes[i]);
        ++i;
    }
    return array_type->GetFullCopy();
}

VariableType* FunctionResult(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* result = CallFunction(lexemes, i, name_space, function_name_space);
    if (lexemes[i] == Type::LeftSquareBracket) {
        VariableType* correct_result = ArrayIndexes(lexemes, i, name_space, function_name_space, result);
        delete result;
        result = correct_result;
    }
    return result;
}

VariableType* Variable(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    VariableType* variable_type = GetVariableType(lexemes[i], name_space);
    ++i;
    if (lexemes[i] == Type::LeftSquareBracket) {
        VariableType* correct_variable_type = ArrayIndexes(lexemes, i, name_space, function_name_space, variable_type);
        delete variable_type;
        return correct_variable_type;
    }
    return variable_type;
}

void Len(std::vector<Lexeme>& lexemes, int& i, 
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
}

VariableType* Expression0(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression1(lexemes, i, name_space, function_name_space);
    while (lexemes[i].value_ == "or") {
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression1(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    return expression_type;
}
VariableType* Expression1(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression2(lexemes, i, name_space, function_name_space);
    if (lexemes[i] == Type::LeftAngleBracket || lexemes[i] == Type::RightAngleBracket
        || lexemes[i].value_ == "!=" || lexemes[i].value_ == "=="
        || lexemes[i].value_ == ">=" || lexemes[i].value_ == "<=") {
        ++i;
        if (!expression_type->IsBaseType())
            throw new ExceptionWrongExpressionType(lexemes[i].line_);
        expression_type = CheckBinExpression(expression_type,
            Expression2(lexemes, i, name_space, function_name_space), lexemes[i].line_);

    }
    return expression_type;
}
VariableType* Expression2(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression3(lexemes, i, name_space, function_name_space);
    while (lexemes[i].value_ == "&" || lexemes[i].value_ == "|" || lexemes[i].value_ == "^") {
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression3(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }

    return expression_type;
}
VariableType* Expression3(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression4(lexemes, i, name_space, function_name_space);
    while (lexemes[i].value_ == "+" || lexemes[i].value_ == "-") {
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression4(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    return expression_type;
}
VariableType* Expression4(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression5(lexemes, i, name_space, function_name_space);
    while (lexemes[i].value_ == "*" || lexemes[i].value_ == "/" || lexemes[i].value_ == "%") {
        ++i;
        expression_type = CheckBinExpression(expression_type,
            Expression5(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    return expression_type;
}
VariableType* Expression5(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "-" && lexemes[i].value_ != "!" && lexemes[i].value_ != "~") {
        return Expression6(lexemes, i, name_space, function_name_space);
    }
    ++i;
    return CheckUnoExpression(Expression6(lexemes, i, name_space, function_name_space), lexemes[i].line_);
}
VariableType* Expression6(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i] == Type::LeftRoundBracket) {
        ++i;
        VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space);
        if (lexemes[i] != Type::RightRoundBracket) {
            throw new ExceptionRightRoundBracket(&lexemes[i]);
        }
        ++i;
        return expression_type;
    }
    if (lexemes[i] == Type::Int || lexemes[i] == Type::Float || lexemes[i] == Type::Bool) {
        ++i;
        return new VariableType(lexemes[i-1].type_);
    }
    if (lexemes[i].value_ == "len") {
        Len(lexemes, i, name_space, function_name_space);
        return new VariableType(VariableTypes::Int);
    }
    if (lexemes[i] == Type::Ident) {
        if (lexemes[i + 1] == Type::LeftRoundBracket) {
            return FunctionResult(lexemes, i, name_space, function_name_space);
        } else {
            return Variable(lexemes, i, name_space, function_name_space);
        }
    }
    throw new Exception("Неправильное выражение", &lexemes[i]);
}
VariableType* Expression(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* expression_type = Expression0(lexemes, i, name_space, function_name_space);
    while (lexemes[i].value_ == "and") {
        ++i;
        expression_type = CheckBinExpression(expression_type, Expression0(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    return expression_type;
}

void Equal(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* variable_type = Variable(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::Equal)
        throw new ExceptionSpecial("=", &lexemes[i]);
    ++i;
    CheckCanDoEqual(variable_type, Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    delete variable_type;
}

void VariableInit(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* variable_type) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], variable_type);
    ++i;
    if (lexemes[i] == Type::Equal) {
        ++i;
        CheckCanDoEqual(variable_type, Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
}

void ArrayInit(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space, VariableType* array_type) {
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], array_type);
    ++i;
    VariableType* final_cell = ArrayIndexes(lexemes, i, name_space, function_name_space, array_type);
    if (!final_cell->IsBaseType())
        throw new ExceptionArrayInitFinalCellNotBased(lexemes[i].line_);
    delete final_cell;
    if (lexemes[i] == Type::Equal) {
        ++i;
        CheckCanDoEqual(array_type, Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
}

void NewVariable(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    VariableType* variable_type = nullptr;
    if (lexemes[i].value_ == "array") {
        variable_type = ArrayType(lexemes, i);
        ArrayInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy());
        while (lexemes[i] == Type::Comma) {
            ++i;
            ArrayInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy());
        }
    } else {
        variable_type = BaseType(lexemes, i);
        VariableInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy());
        while (lexemes[i] == Type::Comma) {
            ++i;
            VariableInit(lexemes, i, name_space, function_name_space, variable_type->GetFullCopy());
        }
    }
    delete variable_type;
}

void If(std::vector<Lexeme>& lexemes, int& i, 
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "if")
        throw new ExceptionSpecial("if", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space);
    if (!expression_type->IsBaseType())
        throw new ExceptionWrongExpressionResult(lexemes[i].line_);
    delete expression_type;
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::Special && lexemes[i].value_ == "else") {
        ++i;
        if (lexemes[i] != Type::LeftBrace)
            throw new ExceptionLeftBrace(&lexemes[i]);
        ++i;
        Block(lexemes, i, name_space, function_name_space);
        if (lexemes[i] != Type::RightBrace)
            throw new ExceptionRightBrace(&lexemes[i]);
        ++i;
    }
}

void While(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "while")
        throw new ExceptionSpecial("while", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    VariableType* expression_type = Expression(lexemes, i, name_space, function_name_space);
    if (!expression_type->IsBaseType())
        throw new ExceptionWrongExpressionResult(lexemes[i].line_);
    delete expression_type;
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
}

void ForVariableInit(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    NewVariable(lexemes, i, name_space, function_name_space);
    while (lexemes[i] == Type::Dot) {
        ++i;
        NewVariable(lexemes, i, name_space, function_name_space);
    }
}

void For(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    name_space = new NameSpace(name_space);
    if (lexemes[i].value_ != "for")
        throw new ExceptionSpecial("for", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::Semicolon) {
        ForVariableInit(lexemes, i, name_space, function_name_space);
    }
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;

    CheckIsResultBasedAndDelete(Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    if (lexemes[i] != Type::Semicolon)
        throw new ExceptionSemicolon(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::RightRoundBracket) {
        Equal(lexemes, i, name_space, function_name_space);
        while (lexemes[i] == Type::Semicolon) {
            ++i;
            Equal(lexemes, i, name_space, function_name_space);
        }
    }

    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;

    if (lexemes[i] != Type::LeftBrace)
        throw new ExceptionLeftBrace(&lexemes[i]);
    ++i;
    Block(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::RightBrace)
        throw new ExceptionRightBrace(&lexemes[i]);
    ++i;
    delete name_space;
}

void Input(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "input")
        throw new ExceptionSpecial("input", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    CheckIsResultBasedAndDelete(Variable(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    while (lexemes[i] == Type::Comma) {
        ++i;
        CheckIsResultBasedAndDelete(Variable(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Print(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "print")
        throw new ExceptionSpecial("print", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    CheckIsResultBasedAndDelete(Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    while (lexemes[i] == Type::Comma) {
        ++i;
        CheckIsResultBasedAndDelete(Expression(lexemes, i, name_space, function_name_space), lexemes[i].line_);
    }
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Return(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i].value_ != "return")
        throw new ExceptionSpecial("return", &lexemes[i]);
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket)
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    ++i;
    if (lexemes[i] == Type::RightRoundBracket) {
        ++i;
        return;
    }
    // TODO: контроль нормальности возврата через CanDoEqual
    Expression(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::RightRoundBracket)
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    ++i;
}

void Action(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    if (lexemes[i] == Type::Special) {
        if (lexemes[i].value_ == "if") {
            If(lexemes, i, name_space, function_name_space);
            return;
        }
        if (lexemes[i].value_ == "while") {
            While(lexemes, i, name_space, function_name_space);
            return;
        }
        if (lexemes[i].value_ == "for") {
            For(lexemes, i, name_space, function_name_space);
            return;
        }
        if (lexemes[i].value_ == "input") {
            Input(lexemes, i, name_space, function_name_space);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "print") {
            Print(lexemes, i, name_space, function_name_space);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        if (lexemes[i].value_ == "return") {
            Return(lexemes, i, name_space, function_name_space);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
            return;
        }
        NewVariable(lexemes, i, name_space, function_name_space);
        if (lexemes[i] != Type::Semicolon)
            throw new ExceptionSemicolon(&lexemes[i]);
        ++i;
        return;
    }
    
    if (lexemes[i] == Type::Ident) {
        if (lexemes[i + 1] == Type::LeftRoundBracket) {
            CallFunction(lexemes, i, name_space, function_name_space);
            if (lexemes[i] != Type::Semicolon)
                throw new ExceptionSemicolon(&lexemes[i]);
            ++i;
        } else {
            Equal(lexemes, i, name_space, function_name_space);
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
    NameSpace* name_space, FunctionNameSpace* function_name_space) {
    NameSpace* new_name_space = new NameSpace(name_space);
    while (lexemes[i] != Type::RightBrace) {
        Action(lexemes, i, new_name_space, function_name_space);
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
    NameSpace* name_space) {
    VariableType* params_type = type(lexemes, i);
    FunctionParameter* params = new FunctionParameter(params_type);
    if (lexemes[i] != Type::Ident)
        throw new ExceptionVariableName(&lexemes[i]);
    name_space->Add(lexemes[i], params_type->GetFullCopy());
    ++i;
    while (lexemes[i] == Type::Comma) {
        ++i;
        params = new FunctionParameter(params_type->GetFullCopy(), params);
        if (lexemes[i] != Type::Ident) {
            throw new ExceptionVariableName(&lexemes[i]);
        }
        name_space->Add(lexemes[i], params_type);
        ++i;
    }
    delete params_type;
    return params;
}

FunctionParameter* FucntionParameters(std::vector<Lexeme>& lexemes, int& i,
    NameSpace* name_space) {
    FunctionParameter* params = FunctionValue(lexemes, i, name_space);
    while (lexemes[i] == Type::Dot) {
        ++i;
        params->AddToEnd(FunctionValue(lexemes, i, name_space));
    }
    return params;
}

void Function(std::vector<Lexeme>& lexemes, int& i, 
    FunctionNameSpace* function_name_space) {
    VariableType* return_value = FunctionType(lexemes, i);
    if (lexemes[i] != Type::Ident)
        throw new ExceptionFunctionName(&lexemes[i]);
    Lexeme* function_name = &lexemes[i];
    ++i;
    if (lexemes[i] != Type::LeftRoundBracket) {
        throw new ExceptionLeftRoundBracket(&lexemes[i]);
    }
    ++i;
    NameSpace* name_space = new NameSpace();
    FunctionParameter* params = nullptr;
    if (lexemes[i] != Type::RightRoundBracket) {
        params = FucntionParameters(lexemes, i, name_space);
    }
    if (lexemes[i] != Type::RightRoundBracket) {
        throw new ExceptionRightRoundBracket(&lexemes[i]);
    }
    ++i;
    if (lexemes[i] != Type::LeftBrace) {
        throw new ExceptionLeftBrace(&lexemes[i]);
    }
    ++i;
    Block(lexemes, i, name_space, function_name_space);
    if (lexemes[i] != Type::RightBrace) {
        throw new ExceptionRightBrace(&lexemes[i]);
    }
    ++i;
    function_name_space->AddFunction(function_name, return_value, params);
}

void SyntaciticAlalysis(std::vector<Lexeme>& lexemes) {
    int i = 0;
    FunctionNameSpace* function_name_space = new FunctionNameSpace();
    while (i != lexemes.size()) {
        Function(lexemes, i, function_name_space);
    }
}