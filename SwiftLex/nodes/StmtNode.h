#pragma once
#include <vector>
#include "idsystem.h"
#include "dotHelpers.h"
#include "ExprNode.h"

enum StmtType
{
	Expr
};

struct StmtNode
{
public:
	int _id = getNewId();

	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;
};

StmtNode* createStmtExpr(ExprNode* expr);

void generateDotStmt(std::ofstream& file, StmtNode* node);

struct StmtListNode
{
public:
	int _id = getNewId();

	std::vector<StmtNode*> _stmtVec = {};
};

StmtListNode* createStmtList(StmtNode* stmt);

StmtListNode* appendStmtToStmtList(StmtListNode* list, StmtNode* stmtToAdd);

void generateDotStmtList(std::ofstream& file, StmtListNode* node);
