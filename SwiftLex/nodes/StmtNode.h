#pragma once
#include <vector>
#include "dottable.h"
#include "GenericListNode.h"
#include "ExprNode.h"
#include "VarDeclarationNode.h"
#include "ReturnNode.h"

enum StmtType
{
	Expr,
	VarDeclarationList,
	Assignment,
	Return
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

	VarDeclarationListNode* _varDeclList;

	static StmtNode* createStmtExpr(ExprNode* expr);

	static StmtNode* createStmtAssignment(ExprNode* left, ExprNode* right);

	static StmtNode* createStmtVarDeclaration(VarDeclarationListNode* varDeclList);

	static StmtNode* createStmtReturn(ReturnNode* ret);

	void generateDot(std::ofstream& file) override;
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>
{
public:
	std::string getName() override;
};
