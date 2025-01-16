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

void FuncCallNode::fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Function call must be associated with a class!");

	if (currentMethod == nullptr)
		throw std::runtime_error("Function call must be inside a method!");

	if (this->_scopeType == normalCall)
	{	
		auto method = currentClass->methods->methods[this->_funcName];
		if (method == nullptr)
			throw std::runtime_error("Method \"" + this->_funcName + "\" is not found in the method table!");

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

		auto funcNameAndTypeRef = currentClass->constants->findOrAddConstant(NameAndType_C, "", 0, 0, method->methodName, method->descriptor);
		auto funcMethodrefRef = currentClass->constants->findOrAddConstant(MethodRef_C, "", 0, 0, currentClass->thisClass, funcNameAndTypeRef);
		this->_methodRef = funcMethodrefRef;
	}
	else
	{
		throw std::runtime_error("Unsupported function call scope with enum type " + std::to_string(this->_scopeType) + "!");
	}
	
}

std::vector<char> FuncCallNode::generateCode(ClassTableElement* currentClass, MethodTableElement* currentMethod)
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
