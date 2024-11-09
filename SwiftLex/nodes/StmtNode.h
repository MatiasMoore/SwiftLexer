#pragma once
#include <vector>
#include "dottable.h"
#include "ExprNode.h"

enum StmtType
{
	Expr
};

struct StmtNode : public Dottable
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

	static StmtNode* createStmtExpr(ExprNode* expr);

	void generateDotExpr(std::ofstream& file) override;
};


struct StmtListNode : public Dottable
{
public:
	std::vector<StmtNode*> _stmtVec = {};

	static StmtListNode* createStmtList(StmtNode* stmt);

	StmtListNode* appendStmt(StmtNode* stmtToAdd);

	void generateDotExpr(std::ofstream& file) override;
};


