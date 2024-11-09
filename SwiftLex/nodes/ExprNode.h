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
	LT,
	GT,
	GTE,
	LTE,
	EQ,
	NEQ,
	BitAnd,
	BitOr,
	BitXor,
	LogAnd,
	LogOr,
	Lshift,
	Rshift,
	ClosedRange,
	HalfOpenRange,
	NilCoalesce,
	BitNot
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
	ExprNode* _unary;

	static ExprNode* createBool(bool value);

	static ExprNode* createInt(int value);

	static ExprNode* createFloat(double value);

	static ExprNode* createString(std::string value);
	
	static ExprNode* createId(std::string value);

	static ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);
	
	static ExprNode* createUnaryOp(ExprType type, ExprNode* unary);

	std::string getName();

	void generateDot(std::ofstream& file) override;
};
