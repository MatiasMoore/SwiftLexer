#include "CaseElementNode.h"
#include "ExprNode.h"
#include "StmtNode.h"

CaseElementNode* CaseElementNode::createCaseExprList(ExprListNode* exprList, StmtListNode* stmtList)
{
    auto node = new CaseElementNode();
    node->_type = CaseElementType::ExprList;
    node->_exprList = exprList;
    node->_stmtList = stmtList;
    printf("N: CaseExprList\n");
    return node;
}

CaseElementNode* CaseElementNode::createCaseDefault(StmtListNode* stmtList)
{
    auto node = new CaseElementNode();
    node->_type = CaseElementType::Default;
    node->_stmtList = stmtList;
    printf("N: CaseDefault\n");
    return node;
}

void CaseElementNode::generateDot(std::ofstream& file)
{
    switch (this->_type)
    {
        case CaseElementType::ExprList:
            file << dotLabel(this->_id, "CaseExprList");
            file << dotConnectionWithLabel(this->_id, this->_exprList->_id, "expected value");
            file << dotConnectionWithLabel(this->_id, this->_stmtList->_id, "body");
            
            this->_exprList->generateDot(file);
            this->_stmtList->generateDot(file);
			break;
		
        case CaseElementType::Default:
			file << dotLabel(this->_id, "CaseDefault");
			file << dotConnectionWithLabel(this->_id, this->_stmtList->_id, "body");
			
			this->_stmtList->generateDot(file);
            break;

        default:
            break;
    }
}

std::string CaseElementListNode::getName()
{
    return "CaseElementsList";
}
