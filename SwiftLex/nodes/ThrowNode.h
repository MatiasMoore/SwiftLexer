#pragma once
#include "dottable.h"

class ExprNode;

class ThrowNode : public Dottable
{
public:
	ExprNode* _expr;

	static ThrowNode* createThrowExpr(ExprNode* expr);

	void generateDot(std::ofstream& file) override;
};
