#include "DoCatchNode.h"
#include "StmtNode.h"
#include "ExprNode.h"
#include "CatchNode.h"

DoCatchNode* DoCatchNode::createOnlyDoNode(StmtListNode* doStmts)
{
    auto node = new DoCatchNode();
    node->_type = DoCatchNodeType::OnlyDo;
    node->_doStmts = doStmts;
    return node;
}

DoCatchNode* DoCatchNode::addCatchNode(CatchNode* catchNode)
{
    this->_type = DoCatchNodeType::DoCatch;
    this->_catchNodes.push_back(catchNode);
    return this;
}

void DoCatchNode::generateDot(std::ofstream& file)
{
    if (this->_doStmts != nullptr)
    {
        file << dotLabel(this->_id, "Do");
        file << dotConnectionWithLabel(this->_id, this->_doStmts->_id, "do branch");
        this->_doStmts->generateDot(file);
    }
    else {
        file << dotLabel(this->_id, "Do (body empty)");
    }

    if (this->_type == DoCatchNodeType::DoCatch) {
        for (CatchNode* node : this->_catchNodes)
        {
            file << dotConnectionWithLabel(this->_id, node->_id, "catch branch");
            node->generateDot(file);
        }
    }
}


