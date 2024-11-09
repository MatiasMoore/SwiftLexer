#include "StmtNode.h"

StmtNode* StmtNode::createStmtExpr(ExprNode* expr)
{
	auto node = new StmtNode();
	node->_type = StmtType::Expr;
	node->_expr = expr;
	printf("N: stmt expr\n");
	return node;
}

void StmtNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case StmtType::Expr:
		file << dotLabel(this->_id, "Stmt");
		file << dotConnection(this->_id, this->_expr->_id);
		this->_expr->generateDot(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

