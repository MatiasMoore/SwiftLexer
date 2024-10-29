#pragma once
#include <iostream>

enum ExprType
{
	Int,
	Float,
	Sum,
	Sub,
	Mul,
	Div,
};

struct ExprNode
{
public:
	ExprType _type;

	int _intValue;
	double _floatValue;

	ExprNode* _left;
	ExprNode* _right;
};

ExprNode* createInt(int value);

ExprNode* createFloat(double value);

ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);



