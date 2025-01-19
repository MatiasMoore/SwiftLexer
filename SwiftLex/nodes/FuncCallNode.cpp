#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "ExprNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"

FuncCallNode* FuncCallNode::createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs)
{
	auto node = new FuncCallNode();
	node->_hasArgs = true;
	node->_scopeType = FuncCallScopeType::normalCall;
	node->_funcName = funcName;
	node->_funcArgs = funcArgs;
	return node;
}

FuncCallNode* FuncCallNode::createFuncCallNoArgs(std::string funcName)
{
	auto node = new FuncCallNode();
	node->_hasArgs = false;
	node->_scopeType = FuncCallScopeType::normalCall;
	node->_funcName = funcName;
	return node;
}

FuncCallNode* FuncCallNode::setAsExprAccess(ExprNode* exprAccess)
{
	this->_scopeType = exprAccessCall;
	_exprAccess = exprAccess;
	return this;
}

std::string FuncCallNode::getName()
{
	std::string scopeType;
	switch (this->_scopeType)
	{
	case FuncCallScopeType::normalCall:
		scopeType = "Normal call";
		break;
	case FuncCallScopeType::selfCall:
		scopeType = "Self call";
		break;
	case FuncCallScopeType::superCall:
		scopeType = "Super call";
		break;
	case FuncCallScopeType::exprAccessCall:
		scopeType = "Expr access call";
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}

	if (this->_hasArgs)
		return scopeType + " with args";
	else
		return scopeType + " with no args";
}

void FuncCallNode::generateDot(std::ofstream& file)
{
	switch (this->_scopeType)
	{
		case FuncCallScopeType::normalCall:
		case FuncCallScopeType::selfCall:
		case FuncCallScopeType::superCall:
		file << dotLabel(this->_id, this->getName() + ": " + _funcName);
		if (this->_hasArgs)
		{
			file << dotConnection(this->_id, _funcArgs->_id);
			this->_funcArgs->generateDot(file);
		}
			break;
		case FuncCallScopeType::exprAccessCall:
		file << dotLabel(this->_id, this->getName() + ": " + _funcName);
		file << dotConnectionWithLabel(this->_id, _exprAccess->_id, "access");
		this->_exprAccess->generateDot(file);
		if (this->_hasArgs)
		{
			file << dotConnection(this->_id, _funcArgs->_id);
			this->_funcArgs->generateDot(file);
		}
			break;
	}
}

SemanticsBase* FuncCallNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);

	if (this->_scopeType == FuncCallScopeType::exprAccessCall)
		this->_exprAccess = this->_exprAccess->semanticsTransform(stack)->typecast<ExprNode>();
	
	if (this->_hasArgs)
		this->_funcArgs = this->_funcArgs->semanticsTransform(stack)->typecast<FuncCallArgListNode>();

	return this;
}

void FuncCallNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Function call must be associated with a class!");

	if (currentMethod == nullptr)
		throw std::runtime_error("Function call must be inside a method!");

	if (this->_scopeType == normalCall)
	{	
		/*
		bool isInternal = currentClass->findInternalMethod(this->_funcName) != nullptr;

		//this node should know class type of method
		//bool isExternal = currentClass->externalMethods->methods.count(this->_funcName) != 0;
		bool isExternal = currentClass->findOrAddInternalMethod(classTable->findClass(classname)->findMethod())

		if (isInternal)
		{
			this->_methodRef = currentClass->findInternalMethod(this->_funcName)->_methodRef;
		}
		else if (isExternal)
		{
			throw std::runtime_error("Unsupported external method call" + LINE_AND_FILE);
			//auto externalMethod = currentClass->externalMethods->methods[this->_funcName];
			//this->_methodRef = externalMethod->_methodRef;
		}
		else
		{
			throw std::runtime_error("Method \"" + this->_funcName + "\" is not found in the method table!");
		}
		*/

		//TODO add arg type checking
		/*
		std::string funcDesc = "(";
		for (auto& callArg : this->_funcArgs->_vec)
		{
			callArg->fillTable(classTable, currentClass, currentMethod);
			funcDesc += descriptorForType(callArg->_argType);
		}
		funcDesc += ")";
		*/

		if (this->_funcName == "print")
		{
			this->_methodRef = classTable->findMethod("print", "(I)V", "InputOutput")->addMethodRefToConstTable(currentClass->getConstTable());
		}
		else
		{
			this->_methodRef = currentClass->findMethod(this->_funcName)->addMethodRefToConstTable(currentClass->getConstTable());
		}


		if (this->_hasArgs) {
			this->_funcArgs->fillTable(classTable, currentClass, currentMethod);
		}

		
	}
	else
	{
		throw std::runtime_error("Unsupported function call scope with enum type " + std::to_string(this->_scopeType) + "!");
	}
	
}

std::vector<char> FuncCallNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	if (this->_scopeType == normalCall)
	{
		// appendVecToVec(code, jvm::aload(0));
		// Static call
		if (this->_hasArgs) {
			appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));
		}
		appendVecToVec(code, jvm::invokestatic(this->_methodRef));
	}
	else
	{
		throw std::runtime_error("Can't generate code for function call \"" + this->_funcName + "\" of enum type " + std::to_string(this->_scopeType) + "!");
	}
	return code;
}
