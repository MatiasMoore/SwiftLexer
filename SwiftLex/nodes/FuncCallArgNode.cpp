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
    if (this->_isAlreadyTransformed)
        return this;

    this->_value = this->_value->semanticsTransform(stack)->typecast<ExprNode>();

    return this;
}

void FuncCallArgNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope)
{
    this->_value->fillTable(classTable, currentClass, currentMethod, currentScope);
    this->_argType = this->_value->evaluateType(classTable, currentClass, currentMethod, currentScope);
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
    return SemanticsBase::semanticsTransformVector<FuncCallArgNode>(stack, this, _vec);
}

void FuncCallArgListNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope)
{
    for (auto& elem : _vec)
    {
        elem->fillTable(classTable, currentClass, currentMethod, currentScope);
    }
}

std::vector<TypeNode*> FuncCallArgListNode::getArgsTypes()
{
    std::vector<TypeNode*> argsTypes = {};
    for (auto& elem : _vec)
	{
		argsTypes.push_back(elem->_argType);
	}
    return argsTypes;
}

std::vector<char> FuncCallArgListNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
    std::vector<char> code = {};
    for (auto& elem : _vec) {
        appendVecToVec(code, elem->generateCode(currentClass, currentMethod));
    }
    return code;
}
