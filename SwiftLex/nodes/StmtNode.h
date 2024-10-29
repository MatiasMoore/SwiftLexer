#pragma once
#include <vector>
#include "ExprNode.h"

enum StmtType
{
	Expr
};

struct StmtNode
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

};

StmtNode* createStmtExpr(ExprNode* expr);

struct StmtListNode
{
public:
	std::vector<StmtNode*> _stmtVec = {};
};

StmtListNode* createStmtList(StmtNode* stmt);

StmtListNode* appendStmtToStmtList(StmtListNode* list, StmtNode* stmtToAdd);

