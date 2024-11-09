#pragma once
#include <vector>
#include "dottable.h"
#include "GenericListNode.h"
#include "ExprNode.h"

enum StmtType
{
	Expr
};

class StmtNode : public Dottable
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

	static StmtNode* createStmtExpr(ExprNode* expr);

	void generateDot(std::ofstream& file) override;
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>
{

};
