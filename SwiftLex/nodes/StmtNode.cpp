#include "StmtNode.h"

StmtNode* StmtNode::createStmtExpr(ExprNode* expr)
{
	auto node = new StmtNode();
	node->_type = StmtType::Expr;
	node->_expr = expr;
	printf("N: stmt expr\n");
	return node;
}

StmtNode* StmtNode::createStmtAssignment(ExprNode* left, ExprNode* right)
{
	auto node = new StmtNode();
	node->_type = StmtType::Assignment;
	node->_assignLeft = left;
	node->_assignRight = right;
	printf("N: stmt assignment\n");
	return node;
}

StmtNode* StmtNode::createStmtVarDeclaration(VarDeclarationListNode* varDeclList)
{
	auto node = new StmtNode();
	node->_type = StmtType::VarDeclarationList;
	node->_varDeclList = varDeclList;
	printf("N: stmt var decl\n");
	return node;
}

void StmtNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case StmtType::Expr:
		file << dotLabel(this->_id, "Stmt");
		file << dotConnection(this->_id, this->_expr->_id);
		this->_expr->generateDot(file);
		break;
	case StmtType::VarDeclarationList:
		file << dotLabel(this->_id, "VarDeclaration");
		file << dotConnection(this->_id, this->_varDeclList->_id);
		this->_varDeclList->generateDot(file);
		break;
	case StmtType::Assignment:
		file << dotLabel(this->_id, "Assignment");
		file << dotConnectionWithLabel(this->_id, this->_assignLeft->_id, "left");
		file << dotConnectionWithLabel(this->_id, this->_assignRight->_id, "right");
		this->_assignLeft->generateDot(file);
		this->_assignRight->generateDot(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

std::string StmtListNode::getName()
{
	return "StmtList";
}
