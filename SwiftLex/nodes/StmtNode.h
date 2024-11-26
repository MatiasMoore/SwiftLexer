#pragma once
#include <vector>
#include "dottable.h"
#include "GenericListNode.h"

class ReturnNode;
class ExprNode;
class LoopNode;
class IfElseNode;
class VarDeclarationListNode;
class FuncDeclNode;
class ThrowNode;
class TryNode;
class DoCatchNode;
class SwitchNode;

enum StmtType
{
	Expr,
	VarDeclarationList,
	Assignment,
	Return,
	Loop,
	IfElse,
	FuncDecl,
	Throw,
	Try,
	Do,
	Switch
};

class StmtNode : public Dottable
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

	ExprNode* _assignLeft;
	ExprNode* _assignRight;

	ReturnNode* _return;

	ThrowNode* _throw;

	DoCatchNode* _do;

	SwitchNode* _switch;

	TryNode* _tryNode;

	LoopNode* _loop;

	IfElseNode* _ifElse;

	VarDeclarationListNode* _varDeclList;

	FuncDeclNode* _funcDecl;

	static StmtNode* createStmtExpr(ExprNode* expr);

	static StmtNode* createStmtAssignment(ExprNode* left, ExprNode* right);

	static StmtNode* createStmtVarDeclaration(VarDeclarationListNode* varDeclList);

	static StmtNode* createStmtReturn(ReturnNode* ret);

	static StmtNode* createStmtLoop(LoopNode* loop);

	static StmtNode* createStmtIfElse(IfElseNode* ifElse);

	static StmtNode* createStmtFuncDecl(FuncDeclNode* funcDecl);

	static StmtNode* createStmtThrow(ThrowNode* throwNode);

	static StmtNode* createStmtTry(TryNode* tryNode);

	static StmtNode* createStmtDo(DoCatchNode* doCatch);

	static StmtNode* createStmtSwitch(SwitchNode* switchNode);

	void generateDot(std::ofstream& file) override;
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>
{
public:
	std::string getName() override;
};
