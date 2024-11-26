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
    switch (this->_type)
    {
        case DoCatchNodeType::OnlyDo:
            file << dotLabel(this->_id, "Do");
            file << dotConnectionWithLabel(this->_id, this->_doStmts->_id, "do branch");
            this->_doStmts->generateDot(file);
	        break;
        case DoCatchNodeType::DoCatch:
            file << dotLabel(this->_id, "Do");
            file << dotConnectionWithLabel(this->_id, this->_doStmts->_id, "do branch");
            this->_doStmts->generateDot(file);
            for (CatchNode* node : this->_catchNodes)
            {
                file << dotConnectionWithLabel(this->_id, node->_id, "catch branch");
                node->generateDot(file);
            }
            
        default:
            printf("Error: DoCatchNode type not recognized\n");
            break;
    }
}


