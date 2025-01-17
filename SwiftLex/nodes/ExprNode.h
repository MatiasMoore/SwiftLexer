#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"

class ExprNode;
class TypeNode;
class FuncCallNode;
class ClassTable;
class ClassTableElement;
class MethodTableElement;

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
	Ternary,
	Array,
	Subscript,
	FuncCall,
	FieldAccess,
	SelfFieldAccess,
	TypeCheck,
	TypeCast,
	TypeCastWithThrow,
	TypeCastWithCheck,
	BinaryNot,
	PrefixPlus,
	PostfixPlus,
	PrefixDiv,
	PostfixDiv,
	PrefixMul,
	PostfixMul,
	PrefixMod,
	PostfixMod,
	PrefixLogAnd,
	PostfixLogAnd,
	PrefixLogOr,
	PostfixLogOr
};

class ExprListNode : public GenericListNode<ExprNode, ExprListNode>
{
public:
	std::string getName() override;
};

class ExprNode : public Dottable, public SemanticsBase
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
	ExprListNode* _arrayExprList;

	std::string _fieldAccessFieldName;
	ExprNode* _fieldAccessExpr;

	ExprNode* _typeCheckCastExpr;
	TypeNode* _typeCheckCastType;

	FuncCallNode* _funcCall;

	int _staticFieldRef;

	static ExprNode* createBool(bool value);

	static ExprNode* createInt(int value);

	static ExprNode* createFloat(double value);

	static ExprNode* createString(std::string value);
	
	static ExprNode* createId(std::string value);

	static ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right);
	
	static ExprNode* createUnaryOp(ExprType type, ExprNode* unary);

	static ExprNode* createTernary(ExprNode* condition, ExprNode* ifTrue, ExprNode* ifFalse);

	static ExprNode* createArray(ExprListNode* list);

	static ExprNode* createFuncCall(FuncCallNode* func);

	static ExprNode* createFieldAccessExpr(ExprNode* expr, std::string fieldName);

	static ExprNode* createFieldAccessSelf(std::string fieldName);

	static ExprNode* createTypeCheck(ExprNode* toCheck, TypeNode* type);

	static ExprNode* createTypeCast(ExprNode* toCheck, TypeNode* type);

	static ExprNode* createTypeCastWithThrow(ExprNode* toCheck, TypeNode* type);

	static ExprNode* createTypeCastWithCheck(ExprNode* toCheck, TypeNode* type);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack);

	TypeNode* evaluateType(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod);

	void fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};
