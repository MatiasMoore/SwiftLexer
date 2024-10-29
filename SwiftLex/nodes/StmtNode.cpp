#include "StmtNode.h"

StmtNode* createStmtExpr(ExprNode* expr)
{
	auto node = new StmtNode();
	node->_type = StmtType::Expr;
	node->_expr = expr;
	printf("N: stmt expr\n");
	return node;
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
