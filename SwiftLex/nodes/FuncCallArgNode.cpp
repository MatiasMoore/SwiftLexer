#include "FuncCallArgNode.h"
#include "ExprNode.h"

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

std::string FuncCallArgListNode::getName()
{
    return "FuncCallArgList";
}
