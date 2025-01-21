#pragma once
#include "dottable.h"
#include "../tables/tables.h"

class ExprNode;

enum ReturnNodeType 
{
	VoidReturn,
	ExprReturn
};

class ReturnNode : public Dottable
{
public:
	ReturnNodeType _type;

	ExprNode* _expr;

	std::string _exprDesc;

	static ReturnNode* createVoidReturn();

	static ReturnNode* createExprReturn(ExprNode* expr);

	void generateDot(std::ofstream& file) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

