#pragma once
#include "dottable.h"

class ExprNode;
class CaseElementNode;
class CaseElementListNode;

class SwitchNode : public Dottable
{
public:
	ExprNode* _expr;
	CaseElementListNode* _caseElementList;

	static SwitchNode* createSwitchNode(ExprNode* expr, CaseElementListNode* caseElementList);

	void generateDot(std::ofstream& file) override;
};

