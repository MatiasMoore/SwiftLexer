#pragma once
#include <vector>
#include "dottable.h"
#include "GenericListNode.h"
#include "ExprNode.h"
#include "VarDeclarationNode.h"

enum StmtType
{
	Expr,
	VarDeclarationList
};

class StmtNode : public Dottable
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

	VarDeclarationListNode* _varDeclList;

	static StmtNode* createStmtExpr(ExprNode* expr);

	static StmtNode* createStmtVarDeclaration(VarDeclarationListNode* varDeclList);

	void generateDot(std::ofstream& file) override;
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>
{
public:
	std::string getName() override;
};
