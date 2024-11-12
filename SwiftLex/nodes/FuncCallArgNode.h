#pragma once
#include "dottable.h"
#include "GenericListNode.h"

class ExprNode;

enum FuncCallArgType
{
	onlyValue,
	valueAndName
};

class FuncCallArgNode : public Dottable
{
public:
	FuncCallArgType _type;

	ExprNode* _value;
	std::string _name;

	static FuncCallArgNode* createFromExpr(ExprNode* expr);

	static FuncCallArgNode* createFromExprAndName(ExprNode* expr, std::string name);

	void generateDot(std::ofstream& file) override;
};

class FuncCallArgListNode : public GenericListNode<FuncCallArgNode, FuncCallArgListNode>
{
	std::string getName() override;
};
