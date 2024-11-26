#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include <vector>

class StmtListNode;
class ExprNode;
class ExprListNode;

enum CaseElementType
{
	ExprList,
	Default
};


class CaseElementNode : public Dottable
{
public:
	CaseElementType _type;

	ExprListNode* _exprList;

	StmtListNode* _stmtList;

	static CaseElementNode* createCaseExprList(ExprListNode* exprList, StmtListNode* stmtList);
	static CaseElementNode* createCaseDefault(StmtListNode* stmtList);

	void generateDot(std::ofstream& file) override;
};

class CaseElementListNode : public GenericListNode<CaseElementNode, CaseElementListNode>
{
public:
	std::string getName() override;
};