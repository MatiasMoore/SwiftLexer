#include "ConstructorDeclNode.h"
#include "FuncDeclArgNode.h"
#include "StmtNode.h"
#include "AccessModifierNode.h"
#include "TypeNode.h"
#include "../generation/generationHelpers.h"
#include "ReturnNode.h"
#include "ClassDeclNode.h"
#include "../ExceptionHelper.h"
#include "ExprNode.h"
#include "FuncCallNode.h"
#include "FuncCallArgNode.h"

ConstructorDeclNode* ConstructorDeclNode::createConstructor(FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException)
{
    auto node = new ConstructorDeclNode();
	
	node->_hasModifiers = false;

	if (argList == nullptr)
	{
		node->_hasArgs = false;
	}
	else
	{
		node->_hasArgs = true;
		node->_argList = argList;
	}

	if (body == nullptr)
	{
		node->_hasBody = false;
	}
	else
	{
		node->_hasBody = true;
		node->_body = body;
	}


	node->_throwsException = throwsException;

    return node;
}

ConstructorDeclNode* ConstructorDeclNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_hasModifiers = true;
	this->_modifiers = modifiers;
	return this;
}

void ConstructorDeclNode::generateDot(std::ofstream& file)
{
	if (this->_hasModifiers)
	{
		file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
		this->_modifiers->generateDot(file);
	}

	std::string extraInfo;
	if (!this->_hasArgs)
		extraInfo += "\nno args";
	if (!this->_hasBody)
		extraInfo += "\nno body";

	extraInfo += (std::string)(this->_throwsException ? "\nthrows exception" : "\ndoesn't throw exception");

	file << dotLabel(this->_id, "ConstructorDecl" + extraInfo);

	if (this->_hasArgs)
	{
		file << dotConnectionWithLabel(this->_id, this->_argList->_id, "args");
		this->_argList->generateDot(file);
	}

	if (this->_hasBody)
	{
		file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
		this->_body->generateDot(file);
	}
}

SemanticsBase* ConstructorDeclNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	if (!this->_hasModifiers)
	{
		//Add default modifiers
		this->_modifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(AccessModifierType::Public));
		this->_hasModifiers = true;
	}

	this->_modifiers = this->_modifiers->semanticsTransform(stack)->typecast<AccessModifierListNode>();
	
	//FIXME
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

	if (parentClass == nullptr)
		throw std::runtime_error("Critical error! Constructor has no parent class!" + LINE_AND_FILE);

	auto baseClassName = parentClass->_classDecl->_baseClassName;
	//Set default basename
	baseClassName = baseClassName.empty() ? "java/lang/Object" : baseClassName;

	//FIXME
	//auto selfId = ExprNode::createId("self");
	//auto baseConstructorArgs = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(selfId));
	//auto baseConstructor = ExprNode::createFuncCall(FuncCallNode::createFuncCall(baseClassName, baseConstructorArgs));
	auto constructorCallStmt = StmtNode::createDefaultClassConstructor(baseClassName);

	//Add default constructor call
	if (this->_hasBody)
	{
		this->_body->appendNodeAtIndex(constructorCallStmt, 0);
	}
	else
	{
		this->_body = StmtListNode::createListNode(constructorCallStmt);
		this->_hasBody = true;
	}

	//Check if function ends with a return
	bool hasReturnStmt = this->_hasBody
		&& this->_body->_vec.back()->_type == StmtType::Return; //last stmt is a return

	if (!hasReturnStmt)
	{
		//Add void return
		auto returnStmt = StmtNode::createStmtReturn(ReturnNode::createVoidReturn());

		if (this->_hasBody)
		{
			this->_body->appendNode(returnStmt);
		}
		else {
			this->_body = StmtListNode::createListNode(returnStmt);
			this->_hasBody = true;
		}
	}

	this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();

	return this;
}

void ConstructorDeclNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Constructor decl must be associated with a class!");

	if (currentMethod != nullptr)
		throw std::runtime_error("Constructor decl can't be inside a method!");

	std::string strDesc = "(";

	if (this->_hasArgs)
	{
		for (auto& arg : this->_argList->_vec)
		{
			strDesc += arg->_argType->toDescriptor();
		}
	}
	strDesc += ")V";
	
	if (!this->_hasModifiers)
		throw std::runtime_error("Constructor decl for class \"" + currentClass->getClassName() + "\" must have access modifiers!");

	currentMethod = currentClass->addInternalMethodToConstantTable("<init>", strDesc, this->_modifiers->getMethodAccessFlags(), this->_body);

	//Default local var for constructors
	currentMethod->getVarTable()->addLocalVar("self", TypeNode::createIdType(currentClass->getClassName())->toDescriptor());

	if (this->_hasArgs)
	{
		for (auto& arg : this->_argList->_vec)
		{
			currentMethod->getVarTable()->addLocalVar(arg->_argName, arg->_argType->toDescriptor());
		}
	}

	if (!this->_hasBody)
		throw std::runtime_error("Constructor decl for class \"" + currentClass->getClassName() + "\" must have a body!");

	this->_body->fillTable(classTable, currentClass, currentMethod);
}
