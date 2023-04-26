#include "SemanticAnalysis.h"

NameSpace::NameSpace(NameSpace* pr) : pr_(pr) {}
NameSpace::~NameSpace() {
    for (auto& el : names_) {
        delete el.second;
    }
}

void NameSpace::Add(Lexeme& lexeme_name, VariableType* type) {
    if (IsInSpace(lexeme_name.value_))
        throw new ExceptionVariableRedeclaration(&lexeme_name);
    names_[lexeme_name.value_] = type;
}

bool NameSpace::IsInSpace(std::string& name) {
    if (names_.find(name) != names_.end())
        return true;
    if (pr_ != nullptr)
        return pr_->IsInSpace(name);
    return false;
}

VariableType* NameSpace::GetVariableType(Lexeme& lexeme_name) {
    if (names_.find(lexeme_name.value_) != names_.end())
        return names_[lexeme_name.value_];
    if (pr_ == nullptr)
        throw new ExceptionVariableUndeclared(&lexeme_name);
    return pr_->GetVariableType(lexeme_name);
}

VariableType::VariableType(VariableTypes type) :
    type_(type), next_(nullptr) {}
VariableType::VariableType(VariableType* next) :
    type_(VariableTypes::Array), next_(next) {}
VariableType::VariableType(Lexeme& type): next_(nullptr) {
    if (type.value_ == "int")
        type_ = VariableTypes::Int;
    else if (type.value_ == "float")
        type_ = VariableTypes::Float;
    else
        type_ = VariableTypes::Bool;
}
VariableType::VariableType(Type type) : next_(nullptr) {
    if (type == Type::Int)
        type_ = VariableTypes::Int;
    else if (type == Type::Float)
        type_ = VariableTypes::Float;
    else
        type_ = VariableTypes::Bool;
}
VariableType::~VariableType() {
    if (next_ != nullptr)
        delete next_;
}

bool VariableType::operator==(VariableType other) {
    if (type_ != other.type_)
        return false;
    return (next_ == other.next_ || (*next_) == (*other.next_));
}
bool VariableType::operator!=(VariableType other) {
    return !(*this == other);
}
bool VariableType::operator>(VariableType other) {
    return type_ > other.type_;
}

bool VariableType::IsBaseType() {
    return type_ == VariableTypes::Int || type_ == VariableTypes::Float || type_ == VariableTypes::Bool;
}
bool VariableType::IsArray() {
    return type_ == VariableTypes::Array;
}

VariableType* VariableType::Next() {
    return next_;
}
VariableType* VariableType::GetFullCopy() {
    if (type_ == VariableTypes::Array)
        return new VariableType(next_->GetFullCopy());
    return new VariableType(type_);
}

FunctionParameter::FunctionParameter(VariableType* value, FunctionParameter* next) :
    value_(value), next_(next) {}
FunctionParameter::~FunctionParameter() {
    delete value_;
    if (next_ != nullptr)
        delete next_;
}

bool FunctionParameter::operator==(FunctionParameter other) {
    if (*value_ != *other.value_)
        return false;
    return next_ == other.next_ || *next_ == *other.next_;
}
bool FunctionParameter::operator!=(FunctionParameter other) {
    return !(*this == other);
}

FunctionNameSpace::FunctionNameSpace(FunctionNameSpace* pr): pr_(pr) {}
FunctionNameSpace::~FunctionNameSpace() {
    for (auto& el : return_value_) {
        delete el.second;
    }
    for (auto& el : parametrs_) {
        delete el.second;
    }
}
void FunctionNameSpace::AddFunction(Lexeme* lexeme_name, 
    VariableType* value_type, FunctionParameter* parametr) {

}
/*bool FunctionNameSpace::IsInSpace(std::string name) {
    if (return_value_.find(name) != return_value_.end())
        return true;
    if (pr_ == nullptr)
        return false;
    return pr_->IsInSpace(name);
}*/
VariableType* FunctionNameSpace::GetFunctionType(Lexeme* lexeme_name) {
    if (return_value_.find(lexeme_name->value_) != return_value_.end())
        return return_value_[lexeme_name->value_];
    if (pr_ == nullptr)
        throw new ExceptionFunctionUndeclared(lexeme_name);
    return pr_->GetFunctionType(lexeme_name);
}
FunctionParameter* FunctionNameSpace::GetFunctionParametrs(Lexeme* lexeme_name) {
    if (parametrs_.find(lexeme_name->value_) != parametrs_.end())
        return parametrs_[lexeme_name->value_];
    if (pr_ == nullptr)
        throw new ExceptionFunctionUndeclared(lexeme_name);
    return pr_->GetFunctionParametrs(lexeme_name);
}

VariableType* CheckBinExpression(VariableType* first, VariableType* second, int line) {
    if (*second > *first)
        std::swap(first, second);
    if (!first->IsBaseType())
        throw new ExceptionWrongExpressionType(line);
    delete second;
    return first;
}
VariableType* CheckUnoExpression(VariableType* type, int line) {
    if (!type->IsBaseType())
        throw new ExceptionWrongExpressionType(line);
    return type;
}
VariableType* GetVariableType(Lexeme& variable_name, NameSpace* name_space) {
    return name_space->GetVariableType(variable_name)->GetFullCopy();
}
void CheckIsResultBasedAndDelete(VariableType* result, int line) {
    if (!result->IsBaseType())
        throw new ExceptionWrongExpressionResult(line);
    delete result;
}
void CheckCanDoEqual(VariableType* left, VariableType* right, int line) {
    if ((left->IsBaseType() && right->IsBaseType()) || *left == *right) {
        delete right;
        return;
    }
    throw new ExceptionWrongExpressionResult(line);
}