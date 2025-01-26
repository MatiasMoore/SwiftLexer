#pragma once
#include "dottable.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

class StmtListNode;
class ExprNode;
class ExprListNode;

class IfElseNode : public Dottable, public SemanticsBase
{
public:
	ExprListNode* _conditions;

	bool _hasIfTrue;
	bool _hasElse;

	StmtListNode* _ifTrue;
	StmtListNode* _else;

	VariableScope* _createdTrueScope;
	VariableScope* _createdElseScope;

	static IfElseNode* createSimple(ExprListNode* conditions, StmtListNode* ifTrue, StmtListNode* else_);

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

