#pragma once
#include "dottable.h"

enum ExprType
{
	Int,
	Float,
	Sum,
	Sub,
	Mul,
	Div,
};

class ExprNode : public Dottable
{
public:
	ExprType _type;

	int _intValue;
	double _floatValue;

	ExprNode* _left;
	ExprNode* _right;

	static ExprNode* createInt(int value);

	static ExprNode* createFloat(double value);

	static ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);

	std::string getName();

	void generateDotExpr(std::ofstream& file) override;
};
