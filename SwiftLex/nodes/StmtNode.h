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
class EnumCaseNode;
class EnumDeclarationNode;
class StructDeclarationNode;
class ConstructorDeclNode;
class DestructorDeclNode;

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
	Switch,
	EnumCase,
	EnumDeclaration,
	StructDeclaration,
	ConstructorDecl,
	DestructorDecl
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

	EnumCaseNode* _enumCase;
	EnumDeclarationNode* _enumDeclaration;

	StructDeclarationNode* _structDeclaration;

	TryNode* _tryNode;

	LoopNode* _loop;

	IfElseNode* _ifElse;

	VarDeclarationListNode* _varDeclList;

	FuncDeclNode* _funcDecl;

	ConstructorDeclNode* _constructorDecl;

	DestructorDeclNode* _destructorDecl;

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

	static StmtNode* createStmtEnumCase(EnumCaseNode* enumCase);

	static StmtNode* createStmtEnumDeclaration(EnumDeclarationNode* enumCase);

	static StmtNode* createStmtStructDeclaration(StructDeclarationNode* structDeclaration);

	static StmtNode* createStmtConstructorDecl(ConstructorDeclNode* constructor);

	static StmtNode* createStmtDestructorDecl(DestructorDeclNode* destructor);

	void generateDot(std::ofstream& file) override;
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>
{
public:
	std::string getName() override;
};
