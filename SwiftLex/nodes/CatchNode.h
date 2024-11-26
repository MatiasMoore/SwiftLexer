#pragma once
#include "dottable.h"

class StmtListNode;
class ExprNode;
class TypeNode;

enum CatchNodeType
{
	Catch,
	CatchFieldAccess,
	CatchType
}; 

class CatchNode : public Dottable
{
public:

	CatchNodeType _type;
	StmtListNode* _stmtList;
	ExprNode* _expr;

	TypeNode* _typeNode;

	static CatchNode* createCatchNode(StmtListNode* stmtList);

	static CatchNode* createCatchFieldAccessNode(ExprNode* expr, StmtListNode* stmtList);

	static CatchNode* createCatchTypeNode(TypeNode* type, StmtListNode* stmtList);

	void generateDot(std::ofstream& file) override;
};

