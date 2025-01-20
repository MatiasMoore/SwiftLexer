#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "ExprNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"
#include "TypeNode.h"

ExternalMethod* FuncCallNode::findMethodForCall(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::string funcCallArgDescriptor = "(";
	if (this->_hasArgs) {
		for (auto arg : this->_funcArgs->_vec)
		{
			funcCallArgDescriptor += arg->_value->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
		}
	}
	funcCallArgDescriptor += ")";
	if (this->_scopeType == normalCall)
	{
		auto classWithFuncName = classTable->findClass(this->_funcName);
		bool isConstructorCall = classWithFuncName != nullptr;

		// Constructor call
		if (isConstructorCall)
		{
			auto constructorMethod = classWithFuncName->findMethod("<init>", funcCallArgDescriptor, false);

			if (constructorMethod == nullptr)
				throw std::runtime_error("Class \"" + this->_funcName + "\" has no constructor with descriptor \"" + funcCallArgDescriptor + "\"!" + LINE_AND_FILE);

			return constructorMethod;
		}

		bool isCurrentMethodStatic = currentMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC);

		bool isCallStatic = isCurrentMethodStatic;

		// Static call
		if (isCallStatic)
		{
			auto methodInThisClass = currentClass->findInternalMethod(this->_funcName, funcCallArgDescriptor, true);

			if (methodInThisClass == nullptr)
			{
				if (!isConstructorCall)
					throw std::runtime_error("Static method \"" + this->_funcName + "\" with descriptor\"" + funcCallArgDescriptor + "\" is not defined in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);
			}

			return methodInThisClass;
		}
		// Dynamic call
		else
		{
			auto methodInThisClass = currentClass->findInternalMethod(this->_funcName, funcCallArgDescriptor, false);

			if (methodInThisClass == nullptr)
			{
				if (!isConstructorCall)
					throw std::runtime_error("Dynamic method \"" + this->_funcName + "\" with descriptor\"" + funcCallArgDescriptor + "\" is not defined in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);
			}

			return methodInThisClass;
		}

	}
	else if (this->_scopeType == exprAccessCall)
	{
		bool isCallStatic = this->_exprAccess->_type == ExprType::Id && classTable->findClass(this->_exprAccess->_stringValue) != nullptr;

		// Static call
		if (isCallStatic)
		{
			auto className = this->_exprAccess->_stringValue;

			auto classElem = classTable->findClass(className);
			if (classElem == nullptr)
				throw std::runtime_error("Unknown identifier \"" + className + "\"in function call!" + LINE_AND_FILE);

			auto method = classElem->findMethod(this->_funcName, funcCallArgDescriptor, true);
			if (method == nullptr)
				throw std::runtime_error("Static method \"" + this->_funcName + "\" with descriptor\"" + funcCallArgDescriptor + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);

			return method;
		}
		// Dynamic call
		else
		{
			auto leftDesc = this->_exprAccess->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
			if (leftDesc[0] != 'L')
				throw std::runtime_error("Primitive types don't have methods!" + LINE_AND_FILE);

			auto className = classnameFromDescriptor(leftDesc);

			auto classElem = classTable->findClass(className);
			if (classElem == nullptr)
				throw std::runtime_error("Critical error! Class \"" + className + "\" is not found!" + LINE_AND_FILE);

			auto method = classElem->findMethod(this->_funcName, funcCallArgDescriptor, false);
			if (method == nullptr)
				throw std::runtime_error("Dynamic method \"" + this->_funcName + "\" with descriptor\"" + funcCallArgDescriptor + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);

			return method;
		}
	}
	else if (this->_scopeType == FuncCallScopeType::selfCall)
	{
		auto selfLocalVar = currentMethod->getVarTable()->findLocalVar("self");
		if (selfLocalVar == nullptr)
			throw std::runtime_error("Keyword \"self\" is only allowed inside non-static methods!");

		if (selfLocalVar->_descriptor[0] != 'L')
			throw std::runtime_error("Critical error! \"Self\" must be a link type!" + LINE_AND_FILE);

		auto localVarClassName = classnameFromDescriptor(selfLocalVar->_descriptor);
		if (localVarClassName != currentClass->getClassName())
			throw std::runtime_error("Critical error! \"Self\" must be of type " + currentClass->getClassName() + " but got " + localVarClassName + "!" + LINE_AND_FILE);

		bool isCurrentMethodStatic = currentMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC);
		if (isCurrentMethodStatic)
			throw std::runtime_error("Critical error! Static methods can't have a \"self\" variable defined!" + LINE_AND_FILE);

		auto method = currentClass->findMethod(this->_funcName, funcCallArgDescriptor, false);
		if (method == nullptr)
			throw std::runtime_error("Dynamic method \"" + this->_funcName + "\" with descriptor\"" + funcCallArgDescriptor + "\" is not defined in class \"" + currentClass->getClassName() + "\"!" + LINE_AND_FILE);

		return method;
	}
	else
	{
		throw std::runtime_error("Unsupported function call type!" + LINE_AND_FILE);
	}
}

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
	if (this->_isAlreadyTransformed)
		return this;

	std::set<std::string> normalCallToInputOutputCall = { "print", "readLine"};

	if (normalCallToInputOutputCall.count(this->_funcName) != 0)
	{
		FuncCallNode* newCall;
		if (this->_hasArgs)
			newCall = FuncCallNode::createFuncCall(this->_funcName, this->_funcArgs);
		else
			newCall = FuncCallNode::createFuncCallNoArgs(this->_funcName);

		newCall->setAsExprAccess(ExprNode::createId("rtl/InputOutput"));
		if (newCall->_scopeType == FuncCallScopeType::exprAccessCall)
			newCall->_exprAccess = newCall->_exprAccess->semanticsTransform(stack)->typecast<ExprNode>();

		if (newCall->_hasArgs)
			newCall->_funcArgs = newCall->_funcArgs->semanticsTransform(stack)->typecast<FuncCallArgListNode>();
		return newCall;
	}
	else
	{
		if (this->_scopeType == FuncCallScopeType::exprAccessCall)
			this->_exprAccess = this->_exprAccess->semanticsTransform(stack)->typecast<ExprNode>();

		if (this->_hasArgs)
			this->_funcArgs = this->_funcArgs->semanticsTransform(stack)->typecast<FuncCallArgListNode>();

		return this;
	}
}

TypeNode* FuncCallNode::evaluateType(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	auto method = this->findMethodForCall(classTable, currentClass, currentMethod);
	bool isConstructor = method->getMethodName() == "<init>";
	if (isConstructor)
	{
		return TypeNode::createIdType(this->_funcName);
	}
	else
	{
		auto returnDesc = method->getReturnDescriptor();
		return TypeNode::createFromDescriptor(returnDesc);
	}
}

void FuncCallNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Function call must be associated with a class!");

	if (currentMethod == nullptr)
		throw std::runtime_error("Function call must be inside a method!");

	if (this->_scopeType == FuncCallScopeType::exprAccessCall)
		this->_exprAccess->fillTable(classTable, currentClass, currentMethod);

	if (this->_hasArgs)
		this->_funcArgs->fillTable(classTable, currentClass, currentMethod);

	auto method = this->findMethodForCall(classTable, currentClass, currentMethod);
	this->_methodRef = currentClass->getMethodRefForExternalMethod(method);
	bool isContructor = method->getMethodName() == "<init>";
	this->_isConstructor = isContructor;
	if (isContructor)
	{
		auto constructorClass = classTable->findClass(this->_funcName);
		this->_classRefForConstructor = currentClass->getClassRefForExternalClass(constructorClass);
	}
	else
	{
		this->_isStaticMethod = method->containsFlag(MethodAccessFlag::M_ACC_STATIC);
	}
}

std::vector<char> FuncCallNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	if (this->_isConstructor)
	{
		appendVecToVec(code, jvm::_new(this->_classRefForConstructor));
		appendVecToVec(code, jvm::dup());

		if (this->_hasArgs)
			appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));

		appendVecToVec(code, jvm::invokespecial(this->_methodRef));
	}
	else
	{		
		if (this->_isStaticMethod)
		{
			if (this->_hasArgs) {
				appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));
			}
			appendVecToVec(code, jvm::invokestatic(this->_methodRef));
		}
		else
		{
			if (this->_scopeType == FuncCallScopeType::selfCall)
			{
				appendVecToVec(code, jvm::aload(0));
			}
			else if (this->_scopeType == FuncCallScopeType::exprAccessCall)
			{
				appendVecToVec(code, this->_exprAccess->generateCode(currentClass, currentMethod));
			}
			else
			{
				throw std::runtime_error("Super calls are not supported yet!" + LINE_AND_FILE);
			}

			if (this->_hasArgs) {
				appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));
			}

			appendVecToVec(code, jvm::invokevirtual(this->_methodRef));
		}
	}
	
	return code;
}
