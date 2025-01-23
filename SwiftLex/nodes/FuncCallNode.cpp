#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "ExprNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"
#include "TypeNode.h"
#include "../GlobalSettings.h"
#include "../RTLHelper.h"

ExternalMethod* FuncCallNode::findMethodForCall(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	
	if (this->_scopeType == normalCall)
	{
		auto classWithFuncName = classTable->findClass(this->_funcName);
		bool isConstructorCall = classWithFuncName != nullptr;

		// Constructor call
		if (isConstructorCall)
		{
			auto constructorMethod = this->findMethodWithTypeCasting(classWithFuncName, "<init>", false, classTable, currentClass, currentMethod);

			if (constructorMethod == nullptr)
				throw std::runtime_error("Class \"" + this->_funcName + "\" has no constructor with descriptor \"" + 
					this->getArgsDescriptor(classTable, currentClass, currentMethod) + "\"!" + LINE_AND_FILE);

			return constructorMethod;
		}

		if (currentMethod == nullptr)
			throw std::runtime_error("This function call \"" + this->_funcName + "\" can't be used in this context" + LINE_AND_FILE);

		bool isCurrentMethodStatic = currentMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC);

		bool isCallStatic = isCurrentMethodStatic;

		// Static call
		if (isCallStatic)
		{
			auto asExternalMethod = this->findMethodWithTypeCasting(currentClass, this->_funcName, true, classTable, currentClass, currentMethod);
			auto methodInThisClass = dynamic_cast<InternalMethod*>(asExternalMethod);

			if (methodInThisClass == nullptr)
			{
				if (!isConstructorCall)
					throw std::runtime_error("Static method \"" + this->_funcName + "\" with descriptor\"" + 
						this->getArgsDescriptor(classTable, currentClass, currentMethod)  + "\" is not defined in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);
			}

			return methodInThisClass;
		}
		// Dynamic call
		else
		{
			auto asExternalMethod = this->findMethodWithTypeCasting(currentClass, this->_funcName, false, classTable, currentClass, currentMethod);
			auto methodInThisClass = dynamic_cast<InternalMethod*>(asExternalMethod);

			if (methodInThisClass == nullptr)
			{
				if (!isConstructorCall)
					throw std::runtime_error("Dynamic method \"" + this->_funcName + "\" with descriptor\"" + 
						this->getArgsDescriptor(classTable, currentClass, currentMethod) + "\" is not defined in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);
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

			auto method = this->findMethodWithTypeCasting(classElem, this->_funcName, true, classTable, currentClass, currentMethod);
			if (method == nullptr)
				throw std::runtime_error("Static method \"" + this->_funcName + "\" with descriptor\"" + 
					this->getArgsDescriptor(classTable, currentClass, currentMethod) + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);

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
			
			auto method = this->findMethodWithTypeCasting(classElem, this->_funcName, false, classTable, currentClass, currentMethod);
			if (method == nullptr)
				throw std::runtime_error("Dynamic method \"" + this->_funcName + "\" with descriptor\"" + 
					this->getArgsDescriptor(classTable, currentClass, currentMethod) + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);

			return method;
		}
	}
	else
	{
		throw std::runtime_error("Unsupported function call type!" + LINE_AND_FILE);
	}
}

std::string FuncCallNode::getArgsDescriptor(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::string funcCallArgDescriptor = "(";
	if (this->_hasArgs) {
		for (auto arg : this->_funcArgs->_vec)
		{
			funcCallArgDescriptor += arg->_value->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
		}
	}
	funcCallArgDescriptor += ")";
	return funcCallArgDescriptor;
}

//TODO support stuff like 1 + 5.0 (exprAccess needs to be casted as well)
ExternalMethod* FuncCallNode::findMethodWithTypeCasting(ExternalClass* classWithMethod, std::string methodName, bool shouldBeStatic, ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	ExternalMethod* method = nullptr;

	auto funcCallArgDescriptor = this->getArgsDescriptor(classTable, currentClass, currentMethod);

	// Just use a perfectly matching method if it's found
	auto perfectMatch = classWithMethod->findMethod(methodName, funcCallArgDescriptor, shouldBeStatic);
	if (perfectMatch != nullptr)
		return perfectMatch;

	int argCount = 0;
	if (this->_hasArgs)
		argCount = this->_funcArgs->_vec.size();

	// No args to try and cast, so there is no way we find anything other than a perfect match (which failed earlier)
	if (argCount == 0)
		return nullptr;

	// Get all methods from class with our name and our amount of arguments
	auto potentialMethods = classWithMethod->getMethodsWithNameAndParamCount(methodName, argCount);
	for (auto& potentialMethod : potentialMethods)
	{
		//Skip if it's not the type we want
		bool isPotentialMethodStatic = potentialMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC);
		if (isPotentialMethodStatic != shouldBeStatic)
			continue;

		//Try to construct required args from what we got

		auto potentialMethodArgs = potentialMethod->getArgDescriptorsVector();

		bool allGood = true; //if its true, we managed to cast everything we needed to
		FuncCallArgListNode* newArgs = this->_funcArgs; //our args, but the ones casted are replaced
		FuncCallArgListNode* onlyNewArgs = nullptr; //only the casted ones

		//For each argument
		for (int argNum = 0; argNum < argCount; argNum++)
		{
			auto ourArg = this->_funcArgs->_vec[argNum];
			auto requiredDesc = potentialMethodArgs[argNum];
			auto ourArgDesc = ourArg->_value->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

			// This arg matches perfectly
			if (requiredDesc == ourArgDesc)
			{
				allGood = true;
				continue;
			}

			bool isRequiredClass = requiredDesc[0] == 'L';
			bool isOurArgClass = ourArgDesc[0] == 'L';
			
			// Check if our can be downcasted to required (for classes)
			if (isRequiredClass && isOurArgClass 
				&& classTable->isClassDerivedFromClass(classnameFromDescriptor(ourArgDesc), classnameFromDescriptor(requiredDesc)))
			{
				allGood = true;
				continue;
			}

			// Check if our can be downcasted to required (for arrays of classes)
			bool isRequiredArray = requiredDesc[0] == '[';
			bool isOurArgArray = ourArgDesc[0] == '[';

			if (isRequiredArray && isOurArgArray)
			{
				bool isRequiredArrayOfClass = requiredDesc.find('L') != -1;
				bool isOurArgArrayOfClass = ourArgDesc.find('L') != -1;

				if (isRequiredArrayOfClass && isOurArgArrayOfClass 
					&& classTable->isClassDerivedFromClass(classnameFromDescriptor(ourArgDesc), classnameFromDescriptor(requiredDesc)))
				{
					allGood = true;
					continue;
				}
			}
			
			// If this is not enabled, stop here
			const std::set<std::string> alwaysCastTheseClasses = {
				RTLHelper::_arrayC, RTLHelper::_baseC, RTLHelper::_floatC, RTLHelper::_boolC, 
				RTLHelper::_intC, RTLHelper::_IOC, RTLHelper::_rangeC, RTLHelper::_strC
			};
			bool shouldAlwaysCastTheseClasses = 
				isRequiredClass
				&& isOurArgClass
				&& alwaysCastTheseClasses.count(classnameFromDescriptor(requiredDesc)) != 0
				&& alwaysCastTheseClasses.count(classnameFromDescriptor(ourArgDesc)) != 0
				;

			if (!GlobalSettings::_IMPLICIT_CONSTRUCTORS && !shouldAlwaysCastTheseClasses)
			{
				allGood = false;
				break;
			}

			// Required arg must be a class since we'll be looking for a constructor
			if (!isRequiredClass)
			{
				allGood = false;
				break;
			}

			// Trying to find constructor from our type to required
			auto requiredClassName = classnameFromDescriptor(requiredDesc);
			auto requiredClass = classTable->findClass(requiredClassName);
			if (requiredClass == nullptr)
				throw std::runtime_error("Critical error!" + LINE_AND_FILE);
			auto ourToRequired = requiredClass->findMethod("<init>", "(" + ourArgDesc + ")", false);

			// Couldn't find the constructor from our type to required
			if (ourToRequired == nullptr)
			{
				allGood = false;
				break;
			}

			// Found the constructor, so use it
			auto ourToRequiredArgs = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ourArg->_value));
			auto ourToRequiredCall = FuncCallNode::createFuncCall(requiredClassName, ourToRequiredArgs);
			auto ourToRequiredExpr = ExprNode::createFuncCall(ourToRequiredCall);
			auto newArg = FuncCallArgNode::createFromExpr(ourToRequiredExpr);;

			//Add to new args
			newArgs->_vec[argNum] = newArg;
			if (onlyNewArgs == nullptr)
				onlyNewArgs = FuncCallArgListNode::createListNode(newArg);
			else
				onlyNewArgs->appendNode(newArg);
		}

		//Success! So now just replace the current args with the new ones
		if (allGood)
		{
			if (newArgs != nullptr)
			{
				this->_funcArgs = newArgs;
				this->_newFuncArgs = onlyNewArgs;
				this->_hasArgs = true;
			}
			return potentialMethod;
		}
	}

	return method;
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

	//Check for permission to call method
	bool isMethodFromOurClass = method->getClassName() == currentClass->getClassName() 
		|| classTable->isClassDerivedFromClass(currentClass->getClassName(), method->getClassName());

	bool isMethodPublic = method->containsFlag(MethodAccessFlag::M_ACC_PUBLIC);

	if (!isMethodPublic && !isMethodFromOurClass)
		throw std::runtime_error("Method \"" + method->getMethodName() + "\" is not public and is inaccessible from this class \"" 
			+ currentClass->getClassName() + "\"!" + LINE_AND_FILE);

	if (this->_newFuncArgs != nullptr)
		this->_newFuncArgs->fillTable(classTable, currentClass, currentMethod);

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

			if (this->_scopeType == FuncCallScopeType::exprAccessCall)
			{
				appendVecToVec(code, this->_exprAccess->generateCode(currentClass, currentMethod));
			}
			else if (this->_scopeType == FuncCallScopeType::normalCall)
			{
				appendVecToVec(code, jvm::aload(0));
			}


			if (this->_hasArgs) {
				appendVecToVec(code, this->_funcArgs->generateCode(currentClass, currentMethod));
			}

			appendVecToVec(code, jvm::invokevirtual(this->_methodRef));
		}
	}
	
	return code;
}
