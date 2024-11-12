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

StmtNode* StmtNode::createStmtReturn(ReturnNode* ret)
{
	auto node = new StmtNode();
	node->_type = StmtType::Return;
	node->_return = ret;
	printf("N: stmt return\n");
	return node;
}

StmtNode* StmtNode::createStmtLoop(LoopNode* loop)
{
	auto node = new StmtNode();
	node->_type = StmtType::Loop;
	node->_loop = loop;
	printf("N: stmt loop\n");
	return node;
}

void StmtNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case StmtType::Expr:
		file << dotLabel(this->_id, "ExprStmt");
		file << dotConnection(this->_id, this->_expr->_id);
		this->_expr->generateDot(file);
		break;
	case StmtType::VarDeclarationList:
		file << dotLabel(this->_id, "VarDeclarationStmt");
		file << dotConnection(this->_id, this->_varDeclList->_id);
		this->_varDeclList->generateDot(file);
		break;
	case StmtType::Assignment:
		file << dotLabel(this->_id, "AssignmentStmt");
		file << dotConnectionWithLabel(this->_id, this->_assignLeft->_id, "left");
		file << dotConnectionWithLabel(this->_id, this->_assignRight->_id, "right");
		this->_assignLeft->generateDot(file);
		this->_assignRight->generateDot(file);
		break;
	case StmtType::Return:
		file << dotLabel(this->_id, "ReturnStmt");
		file << dotConnection(this->_id, this->_return->_id);
		this->_return->generateDot(file);
		break;
	case StmtType::Loop:
		file << dotLabel(this->_id, "LoopStmt");
		file << dotConnection(this->_id, this->_loop->_id);
		this->_loop->generateDot(file);
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
