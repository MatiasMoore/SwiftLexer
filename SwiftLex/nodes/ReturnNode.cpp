#include "ReturnNode.h"
#include "ExprNode.h"
#include "TypeNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"

ReturnNode* ReturnNode::createVoidReturn()
{
    auto node = new ReturnNode();
    node->_type = ReturnNodeType::VoidReturn;
    return node;
}

ReturnNode* ReturnNode::createExprReturn(ExprNode* expr)
{
    auto node = new ReturnNode();
    node->_type = ReturnNodeType::ExprReturn;
    node->_expr = expr;
    return node;
}

void ReturnNode::generateDot(std::ofstream& file)
{
    if (_type == ReturnNodeType::VoidReturn)
    {
        file << dotLabel(this->_id, "Void return");
    }
    else 
    {
        file << dotLabel(this->_id, "Return expr");
        file << dotConnection(this->_id, this->_expr->_id);
        this->_expr->generateDot(file);
    }
}

void ReturnNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
    if (this->_type == ReturnNodeType::ExprReturn)
    {
        this->_expr->fillTable(classTable, currentClass, currentMethod);
        this->_exprDesc = this->_expr->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
    }
}

std::vector<char> ReturnNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
    std::vector<char> code = {};

    if (this->_type == ReturnNodeType::VoidReturn)
    {
        appendVecToVec(code, jvm::_return());
    }
    else if (this->_exprDesc == "I")
    {
        appendVecToVec(code, this->_expr->generateCode(currentClass, currentMethod));
        appendVecToVec(code, jvm::ireturn());
    }
    else if (this->_exprDesc[0] == 'L')
    {
        appendVecToVec(code, this->_expr->generateCode(currentClass, currentMethod));
        appendVecToVec(code, jvm::areturn());
    }
    else
    {
        throw std::runtime_error("Returning a type with a descriptor \"" + this->_exprDesc + "\" is not supported!" + LINE_AND_FILE);
    }

    return code;
}
