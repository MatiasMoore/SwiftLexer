#include "CatchNode.h"
#include "StmtNode.h"
#include "ExprNode.h"
#include "TypeNode.h"

CatchNode* CatchNode::createCatchNode(StmtListNode* stmtList)
{
    auto node = new CatchNode();
    node->_type = CatchNodeType::Catch;
    node->_stmtList = stmtList;
    printf("N: catchNode \n");
    return node;
}

CatchNode* CatchNode::createCatchFieldAccessNode(ExprNode* expr, StmtListNode* stmtList)
{
    auto node = new CatchNode();
    node->_type = CatchNodeType::CatchFieldAccess;
    node->_expr = expr;
    node->_stmtList = stmtList;
    printf("N: catchExprNode \n");
    return node;
}

CatchNode* CatchNode::createCatchTypeNode(TypeNode* type, StmtListNode* stmtList)
{
    auto node = new CatchNode();
    node->_type = CatchNodeType::CatchType;
    node->_typeNode = type;
    node->_stmtList = stmtList;
    printf("N: catchTypeNode \n");
    return node;
}

void CatchNode::generateDot(std::ofstream& file)
{
    switch (this->_type)
    {
        if (this->_stmtList != nullptr)
        {
            file << dotConnectionWithLabel(this->_id, this->_stmtList->_id, "stmtList") << std::endl;
            this->_stmtList->generateDot(file);
		}
       
        case CatchNodeType::Catch:
			file << dotLabel(this->_id, "Catch") << std::endl;
			break;

		case CatchNodeType::CatchFieldAccess:
            file << dotLabel(this->_id, "CatchFieldAccess") << std::endl;
			file << dotConnectionWithLabel(this->_id, this->_expr->_id, "FieldAccess") << std::endl;
			this->_expr->generateDot(file);
            break;

        case CatchNodeType::CatchType:
            file << dotLabel(this->_id, "CatchType") << std::endl;
            file << dotConnectionWithLabel(this->_id, this->_typeNode->_id, "type") << std::endl;
            this->_typeNode->generateDot(file);
            break;

        default:
            break;
    }
}


