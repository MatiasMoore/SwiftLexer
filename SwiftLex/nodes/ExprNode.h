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
	BitNot,
	LogNot,
	UnaryMinus,
	Ternary
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
	ExprNode* _ternaryCondition;
	ExprNode* _ternaryIfTrue;
	ExprNode* _ternaryIfFalse;

	static ExprNode* createBool(bool value);

	static ExprNode* createInt(int value);

	static ExprNode* createFloat(double value);

	static ExprNode* createString(std::string value);
	
	static ExprNode* createId(std::string value);

	static ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);
	
	static ExprNode* createUnaryOp(ExprType type, ExprNode* unary);

	static ExprNode* createTernary(ExprNode* condition, ExprNode* ifTrue, ExprNode* ifFalse);

	std::string getName();

	void generateDot(std::ofstream& file) override;
};
