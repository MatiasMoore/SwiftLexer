#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"

class ExprNode;
class TypeNode;

enum FuncCallArgType
{
	onlyValue,
	valueAndName
};

class FuncCallArgNode : public Dottable, public SemanticsBase
{
public:
	FuncCallArgType _type;

	ExprNode* _value;
	std::string _name;

	TypeNode* _argType;

	static FuncCallArgNode* createFromExpr(ExprNode* expr);

	static FuncCallArgNode* createFromExprAndName(ExprNode* expr, std::string name);

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};

class FuncCallArgListNode : public GenericListNode<FuncCallArgNode, FuncCallArgListNode>, public SemanticsBase
{
	std::string getName() override;
public:

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};
