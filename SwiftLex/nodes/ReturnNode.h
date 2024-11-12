#pragma once
#include "dottable.h"

class ExprNode;

enum ReturnNodeType 
{
	VoidReturn,
	ExprReturn
};

class ReturnNode : public Dottable
{
public:
	ReturnNodeType _type;

	ExprNode* _expr;

	static ReturnNode* createVoidReturn();

	static ReturnNode* createExprReturn(ExprNode* expr);

	void generateDot(std::ofstream& file) override;
};

