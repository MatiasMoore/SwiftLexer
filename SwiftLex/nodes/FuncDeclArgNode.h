#pragma once
#include "dottable.h"
#include "GenericListNode.h"

class TypeNode;
class ExprNode;

enum FuncDeclArgType
{
	positional,
	requiresLabel
};

class FuncDeclArgNode : public Dottable
{
public:
	FuncDeclArgType _type;

	std::string _argName;
	TypeNode* _argType;
	ExprNode* _argDefaultValue;

	bool _hasDefaultValue;

	static FuncDeclArgNode* createPositionalArg(std::string argName, TypeNode* argType, ExprNode* argValue);

	static FuncDeclArgNode* createLabeledArg(std::string argName, TypeNode* argType, ExprNode* argValue);

	void generateDot(std::ofstream& file) override;
};

class FuncDeclArgListNode : public GenericListNode<FuncDeclArgNode, FuncDeclArgListNode>
{
public:
	std::string getName() override;
};

