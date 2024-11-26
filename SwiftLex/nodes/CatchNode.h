#pragma once
#include "dottable.h"

class StmtListNode;
class ExprNode;

enum CatchNodeType
{
	Catch,
	CatchExpr
}; 

class CatchNode : public Dottable
{
public:

	CatchNodeType _type;
	StmtListNode* _stmtList;
	ExprNode* _expr;

	static CatchNode* createCatchNode(StmtListNode* stmtList);

	static CatchNode* createCatchExprNode(ExprNode* expr, StmtListNode* stmtList);

	void generateDot(std::ofstream& file) override;
};

