#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "ExprNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"
#include "TypeNode.h"

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


	//FIXME IN SEMANTICS
	if (this->_funcName == "print")
	{
		auto method = classTable->findMethod("print", "(I)V", "InputOutput");
		this->_methodRef = currentClass->getMethodRefForExternalMethod(method);
		this->_methodFlags = { MethodAccessFlag::M_ACC_STATIC, MethodAccessFlag::M_ACC_PUBLIC };
	}
	else
	{
		if (this->_scopeType == normalCall)
		{
			auto methodInThisClass = currentClass->findInternalMethod(this->_funcName);

			if (methodInThisClass == nullptr)
				throw std::runtime_error("Method \"" + this->_funcName + "\" is not defined in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);

			this->_methodRef = methodInThisClass->_methodRef;
			this->_methodFlags = methodInThisClass->getFlags();
			this->_isConstructor = false;
		}
		else if (this->_scopeType == exprAccessCall)
		{
			auto leftDesc = this->_exprAccess->evaluateType(classTable, currentClass, currentMethod)->toDescriptor(classTable, currentClass, currentMethod);
			if (leftDesc[0] != 'L')
				throw std::runtime_error("Primitive types don't have methods!" + LINE_AND_FILE);

			auto className = classnameFromDescriptor(leftDesc);

			auto classElem = classTable->findClass(className);
			if (classElem == nullptr)
				throw std::runtime_error("Critical error! Class \"" + className + "\" is not found!" + LINE_AND_FILE);

			auto method = classElem->findMethod(this->_funcName);
			if (method == nullptr)
				throw std::runtime_error("Method \"" + this->_funcName + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);

			bool isCallStatic = this->_exprAccess->_type == ExprType::Id && classTable->findClass(this->_exprAccess->_stringValue) != nullptr;
			if (!isCallStatic)
				throw std::runtime_error("Non-static calls are not supported!" + LINE_AND_FILE);

			this->_methodRef = currentClass->getMethodRefForExternalMethod(method);
			this->_methodFlags = method->getFlags();
			this->_isConstructor = false;
		}
		else
		{
			throw std::runtime_error("Unsupported function call type!" + LINE_AND_FILE);
		}
	}

	if (this->_hasArgs) {
		this->_funcArgs->fillTable(classTable, currentClass, currentMethod);
	}
	
}

std::vector<char> FuncCallNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	if (this->_isConstructor)
	{
		throw std::runtime_error("Constructor calls are not supported!" + LINE_AND_FILE);
	}
	else
	{
		//FIXME
		bool isStatic = std::set<MethodAccessFlag>(this->_methodFlags.begin(), this->_methodFlags.end()).count(MethodAccessFlag::M_ACC_STATIC) != 0;
		if (isStatic)
		{
			if (this->_hasArgs) {
				appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));
			}
			appendVecToVec(code, jvm::invokestatic(this->_methodRef));
		}
		else
		{
			throw std::runtime_error("Non-static calls are not supported!" + LINE_AND_FILE);
		}
	}
	
	return code;
}
