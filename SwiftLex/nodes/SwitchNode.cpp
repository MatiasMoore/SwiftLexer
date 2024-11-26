#include "SwitchNode.h"
#include "CaseElementNode.h"
#include "ExprNode.h"

SwitchNode* SwitchNode::createSwitchNode(ExprNode* expr, CaseElementListNode* caseElementList)
{
    auto node = new SwitchNode();
    node->_expr = expr;
    node->_caseElementList = caseElementList;
    return node;
}

void SwitchNode::generateDot(std::ofstream& file)
{
    file << dotLabel(this->_id, "Switch");
    file << dotConnectionWithLabel(this->_id, this->_expr->_id, "expr");
    this->_expr->generateDot(file);
    file << dotConnection(this->_id, this->_caseElementList->_id);
    this->_caseElementList->generateDot(file);
}
