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
#include "ClassDeclNode.h"
#include "TypeNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"
#include <set>

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

StmtNode* StmtNode::createClassDecl(ClassDeclNode* classDecl)
{
	auto node = new StmtNode();
	node->_type = StmtType::ClassDecl;
	node->_classDecl = classDecl;
	printf("N: stmt classDecl\n");
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
		file << dotLabel(this->_id, "ConstructorDeclStmt");
		file << dotConnection(this->_id, this->_constructorDecl->_id);
		this->_constructorDecl->generateDot(file);
		break;
	case StmtType::DestructorDecl:
		file << dotLabel(this->_id, "DestructorDeclStmt");
		file << dotConnection(this->_id, this->_destructorDecl->_id);
		this->_destructorDecl->generateDot(file);
		break;
	case StmtType::ClassDecl:
		file << dotLabel(this->_id, "ClassDeclStmt");
		file << dotConnection(this->_id, this->_classDecl->_id);
		this->_classDecl->generateDot(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

SemanticsBase* StmtNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);

	std::set<StmtType> topLevelStmtTypes = { StmtType::ClassDecl };

	bool isTopLevel = topLevelStmtTypes.count(this->_type) != 0;
	if (!isTopLevel)
	{
		// For functions
		if (this->_type == StmtType::FuncDecl)
		{
			std::set<SemanticsBase*> ignore = {};

			StmtNode* parentClass = nullptr;
			while (parentClass == nullptr)
			{
				auto stmt = stack.getClosest<StmtNode>(ignore);
				if (stmt == nullptr)
					break;

				if (stmt->_type != StmtType::ClassDecl)
				{
					ignore.emplace(stmt);
				}
				else
				{
					parentClass = stmt;
					break;
				}
			}

			bool isInsideClass = parentClass != nullptr && parentClass != this;

			if (!isInsideClass)
				throw std::runtime_error("All function declarations must me inside a class!");
		}
		else
		{
			std::set<SemanticsBase*> ignore = {};

			StmtNode* parentMethod = nullptr;
			while (parentMethod == nullptr)
			{
				auto stmt = stack.getClosest<StmtNode>(ignore);
				if (stmt == nullptr)
					break;

				if (stmt->_type != StmtType::FuncDecl)
				{
					ignore.emplace(stmt);
				}
				else
				{
					parentMethod = stmt;
					break;
				}
			}

			bool isInsideMethod = parentMethod != nullptr && parentMethod != this;

			if (!isInsideMethod)
				throw std::runtime_error("All low level stmts must me inside a method!");
		}
	}

	if (this->_type == StmtType::Expr)
	{
		this->_expr = _expr->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == StmtType::Assignment) {
		this->_assignLeft = _assignLeft->semanticsTransform(stack)->typecast<ExprNode>();
		this->_assignRight = _assignRight->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == StmtType::ClassDecl)
	{
		if (!this->_classDecl->_hasBody)
			throw std::runtime_error("Classes must have a body!");

		this->_classDecl->_body->semanticsTransform(stack);
	}
	else if (this->_type == StmtType::FuncDecl)
	{
		this->_funcDecl->semanticsTransform(stack);
	}
	else if (this->_type == StmtType::VarDeclarationList)
	{
		this->_varDeclList->semanticsTransform(stack);
	}
	else if (this->_type == StmtType::Return)
	{
		//do nothing
	}
	else {
		throw std::runtime_error("Unsupported stmnt with enum type " + std::to_string(this->_type) + "!");
	}
	return this;
}

void StmtNode::fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	switch (this->_type)
	{
	case StmtType::FuncDecl:
		if (currentMethod != nullptr)
			throw std::runtime_error("Nested methods are not supported!");

		if (currentClass == nullptr)
			throw std::runtime_error("Methods must be inside a class!");

		this->_funcDecl->fillTable(classTable, currentClass, currentMethod);
		break;
	case StmtType::ConstructorDecl:
		if (currentMethod != nullptr)
			throw std::runtime_error("Nested constructors are not supported!");

		if (currentClass == nullptr)
			throw std::runtime_error("Constructor must be inside the class!");

		this->_constructorDecl->fillTable(classTable, currentClass, currentMethod);
		break;
	case StmtType::ClassDecl:
		if (currentClass != nullptr)
			throw std::runtime_error("Nested classes are not supported!");

		if (currentMethod != nullptr)
			throw std::runtime_error("Class declaration inside methods is not supported!");

		currentClass = classTable->addClass(this->_classDecl->_name, this->_classDecl->_type == ClassDeclType::HasBaseClass ? this->_classDecl->_baseClassName : "java/lang/Object");

		if (this->_classDecl->_hasBody)
		{
			this->_classDecl->_body->fillTable(classTable, currentClass, currentMethod);
		}
		break;
	case StmtType::Expr:
		if (currentClass == nullptr)
			throw std::runtime_error("Expr stmt must be associated with a class!");

		if (currentMethod == nullptr)
			throw std::runtime_error("Expr stmt must be inside a method!");

		this->_expr->fillTable(classTable, currentClass, currentMethod);
		break;
	case StmtType::VarDeclarationList:
		if (currentClass == nullptr)
			throw std::runtime_error("Expr stmt must be associated with a class!");

		if (currentMethod == nullptr)
			throw std::runtime_error("Expr stmt must be inside a method!");

		this->_varDeclList->fillTable(currentClass, currentMethod);
		break;
	case StmtType::Assignment:
		if (currentClass == nullptr)
			throw std::runtime_error("Expr stmt must be associated with a class!");

		if (currentMethod == nullptr)
			throw std::runtime_error("Expr stmt must be inside a method!");

		this->_assignLeft->fillTable(classTable, currentClass, currentMethod);
		this->_assignRight->fillTable(classTable, currentClass, currentMethod);

		break;
	case StmtType::Return:
		//TODO maybe do something here

		break;
	default:
		throw std::runtime_error("Unsupported stmnt with enum type " + std::to_string(this->_type) + "!");
		break;
	}
}

std::vector<char> StmtNode::generateCode(ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	std::vector<char> code = {};
	switch (this->_type)
	{
	case StmtType::Return:
		//TODO non void returns
		appendVecToVec(code, jvm::_return());
		break;
	case StmtType::Expr:
		appendVecToVec(code, this->_expr->generateCode(currentClass, currentMethod));
		break;
	case VarDeclarationList:
		appendVecToVec(code, this->_varDeclList->generateCode(currentClass, currentMethod));
		break;
	case StmtType::Assignment:

		appendVecToVec(code, this->_assignRight->generateCode(currentClass, currentMethod));

		if (this->_assignLeft->_type == ExprType::Id) {
			int varNum = currentMethod->varTable->findLocalVar(this->_assignLeft->_stringValue)->localId;
			appendVecToVec(code, jvm::istore(varNum));
		}
		else {
			throw std::runtime_error("Unsupported assignment with left operand type: " + std::to_string(this->_assignLeft->_type) + "!");
		}

		break;
	case StmtType::FuncDecl:

		
		break;
	default:
		throw std::runtime_error("Can't generate code for stmnt with type " + std::to_string(this->_type) + "!");
		break;
	}
	return code;
}

std::string StmtListNode::getName()
{
	return "StmtList";
}

SemanticsBase* StmtListNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	/*
	
	for (auto& elem : _vec)
	{
		elem = elem->semanticsTransform(stack)->typecast<StmtNode>();
	}

	*/

	for (int i = 0; i < _vec.size(); i++)
	{
		_vec[i] = _vec[i]->semanticsTransform(stack)->typecast<StmtNode>();
	}

	return this;
}

void StmtListNode::fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(classTable, currentClass, currentMethod);
	}
}
