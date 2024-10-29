#include "StmtNode.h"

StmtNode* createStmtExpr(ExprNode* expr)
{
	auto node = new StmtNode();
	node->_type = StmtType::Expr;
	node->_expr = expr;
	printf("N: stmt expr\n");
	return node;
}

void generateDotStmt(std::ofstream& file, StmtNode* node)
{
	switch (node->_type)
	{
	case StmtType::Expr:
		file << dotLabel(node->_id, "Stmt");
		file << dotConnection(node->_id, node->_expr->_id);
		generateDotExpr(file, node->_expr);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

StmtListNode* createStmtList(StmtNode* stmt)
{
	auto node = new StmtListNode();
	node->_stmtVec.push_back(stmt);
	printf("N: new stmt list\n");
	return node;
}

StmtListNode* appendStmtToStmtList(StmtListNode* list, StmtNode* stmtToAdd)
{
	list->_stmtVec.push_back(stmtToAdd);
	printf("N: append to stmt list\n");
	return list;
}

void generateDotStmtList(std::ofstream& file, StmtListNode* node)
{
	file << dotLabel(node->_id, "Stmt list");
	for (auto& child : node->_stmtVec)
	{
		file << dotConnection(node->_id, child->_id);
		generateDotStmt(file, child);
	}
}
