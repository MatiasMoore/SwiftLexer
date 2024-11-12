#include "ReturnNode.h"
#include "ExprNode.h"

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
