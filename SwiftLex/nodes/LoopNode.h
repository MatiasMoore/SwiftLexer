#pragma once
#include "dottable.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

class StmtListNode;
class ExprNode;
class ExprListNode;

enum LoopNodeType
{
	forLoop,
	repeatWhileLoop,
	whileLoop
};

class LoopNode : public Dottable, public SemanticsBase
{
public:
	LoopNodeType _type;
	bool _hasBody;

	std::string _forLoopId;
	ExprNode* _forLoopIterable;
	StmtListNode* _body;
	ExprListNode* _conditions;

	static LoopNode* createForLoop(std::string id, ExprNode* iterable, StmtListNode* body);

	static LoopNode* createForLoopNoBody(std::string id, ExprNode* iterable);

	static LoopNode* createRepeatWhileLoop(StmtListNode* body, ExprListNode* condition);

	static LoopNode* createRepeatWhileLoopNoBody(ExprListNode* condition);

	static LoopNode* createWhileLoop(ExprListNode* condition, StmtListNode* body);

	static LoopNode* createWhileLoopNoBody(ExprListNode* condition);

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

