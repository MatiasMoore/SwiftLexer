#pragma once
#include "dottable.h"
#include "ExprNode.h"

class StmtListNode;

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

