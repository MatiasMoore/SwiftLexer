#pragma once
#include "dottable.h"

class StmtListNode;
class ExprNode;
class ExprListNode;

enum IfElseNodeType
{
	onlyIf,
	ifElseSimple,
	ifElseComplex
};

class IfElseNode : public Dottable
{
public:
	IfElseNodeType _type;

	ExprListNode* _conditions;

	bool _hasIfTrue;
	bool _hasElse;

	StmtListNode* _ifTrue;
	StmtListNode* _elseSimple;
	IfElseNode* _elseComplex;

	static IfElseNode* createSimple(ExprListNode* conditions, StmtListNode* ifTrue, StmtListNode* elseSimple);

	static IfElseNode* createComplex(ExprListNode* conditions, StmtListNode* ifTrue, IfElseNode* elseComplex);

	void generateDot(std::ofstream& file) override;
};

