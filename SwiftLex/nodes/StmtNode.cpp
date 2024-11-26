#include "StmtNode.h"
#include "ExprNode.h"
#include "VarDeclarationNode.h"
#include "ReturnNode.h"
#include "LoopNode.h"
#include "IfElseNode.h"
#include "FuncDeclNode.h"
#include "ThrowNode.h"
#include "TryNode.h"
#include "DoCatchNode.h"
#include "SwitchNode.h"
#include "EnumCaseNode.h"
#include "EnumDeclarationNode.h"
#include "StructDeclarationNode.h"
#include "ConstructorDeclNode.h"
#include "DestructorDeclNode.h"

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

StmtNode* StmtNode::createStmtIfElse(IfElseNode* ifElse)
{
	auto node = new StmtNode();
	node->_type = StmtType::IfElse;
	node->_ifElse = ifElse;
	printf("N: stmt ifElse\n");
	return node;
}

StmtNode* StmtNode::createStmtFuncDecl(FuncDeclNode* funcDecl)
{
	auto node = new StmtNode();
	node->_type = StmtType::FuncDecl;
	node->_funcDecl = funcDecl;
	printf("N: stmt funcDecl\n");
	return node;
}

StmtNode* StmtNode::createStmtThrow(ThrowNode* throwNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Throw;
	node->_throw = throwNode;
	printf("N: stmt throw\n");
	return node;
}

StmtNode* StmtNode::createStmtTry(TryNode* tryNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Try;
	node->_tryNode = tryNode;
	printf("N: stmt throw\n");
	return node;
}

StmtNode* StmtNode::createStmtDo(DoCatchNode* doCatch)
{
	auto node = new StmtNode();
	node->_type = StmtType::Do;
	node->_do = doCatch;
	printf("N: stmt doCatch\n");
	return node;
}

StmtNode* StmtNode::createStmtSwitch(SwitchNode* switchNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Switch;
	node->_switch = switchNode;
	printf("N: stmt switch\n");
	return node;
}

StmtNode* StmtNode::createStmtEnumCase(EnumCaseNode* enumCase)
{
	auto node = new StmtNode();
	node->_type = StmtType::EnumCase;
	node->_enumCase = enumCase;
	printf("N: stmt enumCase\n");
	return node;
}

StmtNode* StmtNode::createStmtEnumDeclaration(EnumDeclarationNode* enumDeclaration)
{
	auto node = new StmtNode();
	node->_type = StmtType::EnumDeclaration;
	node->_enumDeclaration = enumDeclaration;
	printf("N: stmt enumDeclaration\n");
	return node;
}

StmtNode* StmtNode::createStmtStructDeclaration(StructDeclarationNode* structDeclaration)
{
	auto node = new StmtNode();
	node->_type = StmtType::StructDeclaration;
	node->_structDeclaration = structDeclaration;
	printf("N: stmt structDeclaration\n");
	return node;
}

StmtNode* StmtNode::createStmtConstructorDecl(ConstructorDeclNode* constructor)
{
	auto node = new StmtNode();
	node->_type = StmtType::ConstructorDecl;
	node->_constructorDecl = constructor;
	printf("N: stmt constructorDecl\n");
	return node;
}

StmtNode* StmtNode::createStmtDestructorDecl(DestructorDeclNode* destructor)
{
	auto node = new StmtNode();
	node->_type = StmtType::DestructorDecl;
	node->_destructorDecl = destructor;
	printf("N: stmt destructorDecl\n");
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
	case StmtType::IfElse:
		file << dotLabel(this->_id, "IfElseStmt");
		file << dotConnection(this->_id, this->_ifElse->_id);
		this->_ifElse->generateDot(file);
		break;
	case StmtType::FuncDecl:
		file << dotLabel(this->_id, "FuncDeclStmt");
		file << dotConnection(this->_id, this->_funcDecl->_id);
		this->_funcDecl->generateDot(file);
		break;
	case StmtType::Throw:
		file << dotLabel(this->_id, "ThrowStmt");
		file << dotConnection(this->_id, this->_throw->_id);
		this->_throw->generateDot(file);
		break;
	case StmtType::Try:
		file << dotLabel(this->_id, "TryStmt");
		file << dotConnection(this->_id, this->_tryNode->_id);
		this->_tryNode->generateDot(file);
		break;
	case StmtType::Do:
		file << dotLabel(this->_id, "DoStmt");
		file << dotConnection(this->_id, this->_do->_id);
		this->_do->generateDot(file);
		break;
	case StmtType::Switch:
		file << dotLabel(this->_id, "SwitchStmt");
		file << dotConnection(this->_id, this->_switch->_id);
		this->_switch->generateDot(file);
		break;
	case StmtType::EnumCase:
		file << dotLabel(this->_id, "EnumCaseStmt");
		file << dotConnection(this->_id, this->_enumCase->_id);
		this->_enumCase->generateDot(file);
		break;
	case StmtType::EnumDeclaration:
		file << dotLabel(this->_id, "EnumDeclarationStmt");
		file << dotConnection(this->_id, this->_enumDeclaration->_id);
		this->_enumDeclaration->generateDot(file);
		break;
	case StmtType::StructDeclaration:
		file << dotLabel(this->_id, "StructDeclarationStmt");
		file << dotConnection(this->_id, this->_structDeclaration->_id);
		this->_structDeclaration->generateDot(file);
		break;
	case StmtType::ConstructorDecl:
		file << dotLabel(this->_id, "ConstructorDecl");
		file << dotConnection(this->_id, this->_constructorDecl->_id);
		this->_constructorDecl->generateDot(file);
		break;
	case StmtType::DestructorDecl:
		file << dotLabel(this->_id, "DestructorDecl");
		file << dotConnection(this->_id, this->_destructorDecl->_id);
		this->_destructorDecl->generateDot(file);
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
