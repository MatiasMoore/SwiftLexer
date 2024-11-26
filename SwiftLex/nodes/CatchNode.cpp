#include "CatchNode.h"
#include "StmtNode.h"
#include "ExprNode.h"

CatchNode* CatchNode::createCatchNode(StmtListNode* stmtList)
{
    auto node = new CatchNode();
    node->_type = CatchNodeType::Catch;
    node->_stmtList = stmtList;
    printf("N: catchNode \n");
    return node;
}

CatchNode* CatchNode::createCatchExprNode(ExprNode* expr, StmtListNode* stmtList)
{
    auto node = new CatchNode();
    node->_type = CatchNodeType::CatchExpr;
    node->_expr = expr;
    node->_stmtList = stmtList;
    printf("N: catchExprNode \n");
    return node;
}

void CatchNode::generateDot(std::ofstream& file)
{
    switch (this->_type)
    {
        case CatchNodeType::Catch:
			file << dotLabel(this->_id, "Catch") << std::endl;
            file << dotConnectionWithLabel(this->_id, this->_stmtList->_id, "stmtList") << std::endl;
            this->_stmtList->generateDot(file);
			break;

		case CatchNodeType::CatchExpr:
            file << dotLabel(this->_id, "CatchExpr") << std::endl;
			file << dotConnectionWithLabel(this->_id, this->_expr->_id, "expr") << std::endl;
			file << dotConnectionWithLabel(this->_id, this->_stmtList->_id, "stmtList") << std::endl;
			this->_expr->generateDot(file);
			this->_stmtList->generateDot(file);
            break;

        default:
            break;
    }
}


