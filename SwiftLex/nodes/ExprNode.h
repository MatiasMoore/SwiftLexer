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
	String,
	Id,
	Bool,
	Modulus,
	LT
};

class ExprNode : public Dottable
{
public:
	ExprType _type;

	bool _boolValue;
	int _intValue;
	double _floatValue;
	std::string _stringValue;

	ExprNode* _left;
	ExprNode* _right;

	static ExprNode* createBool(bool value);

	static ExprNode* createInt(int value);

	static ExprNode* createFloat(double value);

	static ExprNode* createString(std::string value);
	
	static ExprNode* createId(std::string value);

	static ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);

	std::string getName();

	void generateDot(std::ofstream& file) override;
};
