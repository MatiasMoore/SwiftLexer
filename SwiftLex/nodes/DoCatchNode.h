#pragma once
#include "dottable.h"
#include <vector>

class StmtListNode;
class ExprNode;
class CatchNode;

enum DoCatchNodeType
{
	OnlyDo,
	DoCatch
};



class DoCatchNode : public Dottable
{
public:

	DoCatchNodeType _type;

	StmtListNode* _doStmts;

	std::vector<CatchNode*> _catchNodes;

	static DoCatchNode* createOnlyDoNode(StmtListNode* doStmts);

	DoCatchNode* addCatchNode(CatchNode* catchNode);

	void generateDot(std::ofstream& file) override;


};

