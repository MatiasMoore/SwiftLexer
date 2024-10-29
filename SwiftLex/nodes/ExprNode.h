#pragma once
#include <iostream>
#include "idsystem.h"
#include "dotHelpers.h"

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
	int _id = getNewId();

	ExprType _type;

	int _intValue;
	double _floatValue;

	ExprNode* _left;
	ExprNode* _right;
};

ExprNode* createInt(int value);

ExprNode* createFloat(double value);

ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);

void generateDotExpr(std::ofstream& file, ExprNode* node);
