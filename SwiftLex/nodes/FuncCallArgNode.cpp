#include "FuncCallArgNode.h"
#include "ExprNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"

FuncCallArgNode* FuncCallArgNode::createFromExpr(ExprNode* expr)
{
    auto node = new FuncCallArgNode();
    node->_type = onlyValue;
    node->_value = expr;
    return node;
}

FuncCallArgNode* FuncCallArgNode::createFromExprAndName(ExprNode* expr, std::string name)
{
    auto node = new FuncCallArgNode();
    node->_type = valueAndName;
    node->_value = expr;
    node->_name = name;
    return node;
}

void FuncCallArgNode::generateDot(std::ofstream& file)
{
    if (_type == onlyValue)
        file << dotLabel(this->_id, "Positional arg");
    else
        file << dotLabel(this->_id, "Named arg: " + _name);

    file << dotConnection(this->_id, _value->_id);
    this->_value->generateDot(file);
}

SemanticsBase* FuncCallArgNode::semanticsTransform(SemanticsStack stack)
{
    stack.push(this);

    this->_value = this->_value->semanticsTransform(stack)->typecast<ExprNode>();

    return this;
}

void FuncCallArgNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
    this->_value->fillTable(classTable, currentClass, currentMethod);
    this->_argType = this->_value->evaluateType(classTable, currentClass, currentMethod);
}

std::vector<char> FuncCallArgNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
    std::vector<char> code = {};
    appendVecToVec(code, this->_value->generateCode(currentClass, currentMethod));
    return code;
}

std::string FuncCallArgListNode::getName()
{
    return "FuncCallArgList";
}

SemanticsBase* FuncCallArgListNode::semanticsTransform(SemanticsStack stack)
{
    stack.push(this);
    for (int i = 0; i < _vec.size(); i++)
    {
        _vec[i] = _vec[i]->semanticsTransform(stack)->typecast<FuncCallArgNode>();
    }
    return this;
}

void FuncCallArgListNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
    for (auto& elem : _vec)
    {
        elem->fillTable(classTable, currentClass, currentMethod);
    }
}

std::vector<char> FuncCallArgListNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
    std::vector<char> code = {};
    for (auto& elem : _vec) {
        appendVecToVec(code, elem->generateCode(currentClass, currentMethod));
    }
    return code;
}
