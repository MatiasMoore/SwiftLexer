#pragma once
#include "dottable.h"

class StmtListNode;
class ExprNode;
class ExprListNode;

enum LoopNodeType
{
	forLoop,
	repeatWhileLoop,
	whileLoop
};

class LoopNode : public Dottable
{
public:
	LoopNodeType _type;
	bool _hasBody;

	std::string _forLoopId;
	ExprNode* _forLoopIterable;
	StmtListNode* _body;
	ExprListNode* _condition;

	static LoopNode* createForLoop(std::string id, ExprNode* iterable, StmtListNode* body);

	static LoopNode* createForLoopNoBody(std::string id, ExprNode* iterable);

	static LoopNode* createRepeatWhileLoop(StmtListNode* body, ExprListNode* condition);

	static LoopNode* createRepeatWhileLoopNoBody(ExprListNode* condition);

	static LoopNode* createWhileLoop(ExprListNode* condition, StmtListNode* body);

	static LoopNode* createWhileLoopNoBody(ExprListNode* condition);

	void generateDot(std::ofstream& file) override;
};

