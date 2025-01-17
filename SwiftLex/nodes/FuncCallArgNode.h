#pragma once
#include "dottable.h"
#include "GenericListNode.h"

class ExprNode;
class TypeNode;

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

	TypeNode* _argType;

	static FuncCallArgNode* createFromExpr(ExprNode* expr);

	static FuncCallArgNode* createFromExprAndName(ExprNode* expr, std::string name);

	void generateDot(std::ofstream& file) override;

	void fillTable(class ClassTable* classTable, class ClassTableElement* currentClass, class MethodTableElement* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};

class FuncCallArgListNode : public GenericListNode<FuncCallArgNode, FuncCallArgListNode>
{
	std::string getName() override;
public:
	void fillTable(class ClassTable* classTable, class ClassTableElement* currentClass, class MethodTableElement* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};
