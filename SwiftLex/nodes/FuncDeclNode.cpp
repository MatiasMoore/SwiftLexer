#include "FuncDeclNode.h"
#include "StmtNode.h"
#include "FuncDeclArgNode.h"
#include "TypeNode.h"
#include "AccessModifierNode.h"
#include "ReturnNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"
#include "../RTLHelper.h"

FuncDeclNode* FuncDeclNode::createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isOperatorOverload = false;
	node->_idName = idName;
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

	if (returnType == nullptr)
	{
		node->_hasNonVoidReturn = false;
	}
	else
	{
		node->_hasNonVoidReturn = true;
		node->_returnType = returnType;
	}

	node->_throwsException = throwsException;

	return node;
}

FuncDeclNode* FuncDeclNode::createRegularOperator(OverloadOperatorNotation overloadNotation, OverloadableOperatorType overloadOperatorType, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isOperatorOverload = true;
	node->_overloadOperatorType = overloadOperatorType;
	node->_overloadNotation = overloadNotation;
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

	if (returnType == nullptr)
	{
		node->_hasNonVoidReturn = false;
	}
	else
	{
		node->_hasNonVoidReturn = true;
		node->_returnType = returnType;
	}

	node->_throwsException = throwsException;

	return node;
}

FuncDeclNode* FuncDeclNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_hasModifiers = true;
	this->_modifiers = modifiers;
	return this;
}

#define getOperatorOverloadNameError "Unsupported OverloadOperatorNotation with enum value " + std::to_string(overloadOperatorNotation) + " for OverloadableOperatorType with enum value " + std::to_string(overloadOperatorType) + LINE_AND_FILE

std::string FuncDeclNode::getOperatorOverloadName(OverloadableOperatorType overloadOperatorType, OverloadOperatorNotation overloadOperatorNotation)
{
	switch (this->_overloadOperatorType)
	{
		case (OverloadableOperatorType::OpPLUS):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_sum;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
				break;
		case (OverloadableOperatorType::OpMINUS):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_sub;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpMUL):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_mul;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpDIV):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_div;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpMOD):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpLT):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_lt;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpGT):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_gt;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpGTE):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_gte;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpLTE):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_lte;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpEQ):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_eq;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpNEQ):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_neq;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpBitAND):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpBitOR):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpBitXOR):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpLogAND):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_logAnd;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpLogOR):
			switch (overloadOperatorNotation)
			{
			case Infix:
				return RTLHelper::_logOr;
				break;
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpLSHIFT):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpRSHIFT):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpCLOSEDRANGE):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpHALFOPENRANGE):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		case (OverloadableOperatorType::OpNILCOALESCE):
			switch (overloadOperatorNotation)
			{
			default:
				throw std::runtime_error(getOperatorOverloadNameError);
				break;
			}
			break;
		default:
			throw std::runtime_error("Unsupported OverloadableOperatorType with enum value " + std::to_string(overloadOperatorType) + LINE_AND_FILE);
			break;
	}
}

void FuncDeclNode::generateDot(std::ofstream& file)
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
	if (!this->_hasNonVoidReturn)
		extraInfo += "\nvoid return";
	
	extraInfo += (std::string)(this->_throwsException ? "\nthrows exception" : "\ndoesn't throw exception");

	if (this->_isOperatorOverload)
	{
		file << dotLabel(this->_id, "OperatorOverloadDecl\noperatorName: " + this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation) + extraInfo);
	}
	else
	{
		file << dotLabel(this->_id, "FuncDecl\nidName: " + this->_idName + extraInfo);
	}

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

	if (this->_hasNonVoidReturn)
	{
		file << dotConnectionWithLabel(this->_id, this->_returnType->_id, "return type");
		this->_returnType->generateDot(file);
	}

}

void FuncDeclNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, bool initialScan)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Func decl must be associated with a class!");

	if (currentMethod != nullptr)
		throw std::runtime_error("Func decl can't be inside a method!");

	if (initialScan)
	{
		auto accessFlags = this->_modifiers->getMethodAccessFlags();
		bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();
		std::string funcName;

		if (this->_isOperatorOverload)
		{
			if (!this->_hasModifiers)
				throw std::runtime_error("Operator overload \"" + this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation) + "\" must have access modifiers!" + LINE_AND_FILE);

			if (!this->_hasArgs)
				throw std::runtime_error("Operator overload \"" + this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation) + "\" must have arguments!" + LINE_AND_FILE);

			funcName = this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation);

			currentClass = classTable->findInternalClass(RTLHelper::_internalOpClassName);

			if (currentClass == nullptr)
				throw std::runtime_error("Critical error! Can't find internal operators class named \"" + RTLHelper::_internalOpClassName + "\"!" + LINE_AND_FILE);
		}
		else
		{
			if (!this->_hasModifiers)
				throw std::runtime_error("Func decl \"" + this->_idName + "\" must have access modifiers!");

			funcName = this->_idName;

			if (isStatic && this->_idName != RTLHelper::_defaultMainFunc)
				funcName += "$";
		}

		std::string fullDesc = "";
		std::string argDesc = "(";

		if (this->_hasArgs)
		{
			for (auto& arg : this->_argList->_vec)
			{
				argDesc += arg->_argType->toDescriptor();
			}
		}
		argDesc += ")";

		fullDesc = argDesc;

		if (this->_hasNonVoidReturn)
			fullDesc += this->_returnType->toDescriptor();
		else
			fullDesc += "V";

		bool isOverride = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_CUSTOM_OVERRIDE) != accessFlags.end();
		std::string prefix = isStatic ? "static" : "non-static";

		auto existingMethod = currentClass->findMethod(funcName, argDesc, isStatic);

		if (existingMethod == nullptr && isOverride)
			throw std::runtime_error("Class \"" + currentClass->getClassName() + "\" can't override a " + prefix + " method with name \"" +
				funcName + "\" and arg descriptor \"" + argDesc + "\" as it's baseclass doesn't have it!" + LINE_AND_FILE);

		// This method already exist in this class
		if (existingMethod != nullptr)
		{
			auto asInternalMethod = dynamic_cast<InternalMethod*>(existingMethod);

			if (asInternalMethod != nullptr)
			{
				throw std::runtime_error("Class \"" + currentClass->getClassName() + "\" already has a " + prefix + " method with name \"" +
					funcName + "\" and arg descriptor \"" + argDesc + "\" defined!" + LINE_AND_FILE);
			}

			if (!isOverride)
				throw std::runtime_error("Class \"" + currentClass->getClassName() + "\" already has a " + prefix + " method with name \"" +
					funcName + "\" and arg descriptor \"" + argDesc +
					"\" defined in it's base class! If you want to override it, use the keyword override!" + LINE_AND_FILE);

			currentMethod = currentClass->addInternalMethodAsOverrideToConstantTable(existingMethod, funcName, fullDesc, accessFlags, this->_body);
		}
		// This method is new for this class
		else
		{
			currentMethod = currentClass->addInternalMethodToConstantTable(funcName, fullDesc, accessFlags, this->_body);
		}

		if (!isStatic)
		{
			currentMethod->getVarTable()->addLocalVar("self", TypeNode::createIdType(currentClass->getClassName())->toDescriptor());
		}

		if (this->_hasArgs)
		{
			for (auto& arg : this->_argList->_vec)
			{
				currentMethod->getVarTable()->addLocalVar(arg->_argName, arg->_argType->toDescriptor());
			}
		}

		this->_scannedMethod = currentMethod;
	}
	else
	{
		if (this->_isOperatorOverload)
		{
			if (!this->_hasBody)
				throw std::runtime_error("Operator overload \"" + this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation) + "\" must have a body!");

			if (this->_scannedMethod == nullptr)
				throw std::runtime_error("Critical error! Initial scan failed for operator overload \"" + this->getOperatorOverloadName(this->_overloadOperatorType, this->_overloadNotation) + "\" of class \"" + currentClass->getClassName() + "\"!" + LINE_AND_FILE);
		
			currentClass = classTable->findInternalClass(RTLHelper::_internalOpClassName);

			if (currentClass == nullptr)
				throw std::runtime_error("Critical error! Can't find internal operators class named \"" + RTLHelper::_internalOpClassName + "\"!" + LINE_AND_FILE);
		}
		else
		{
			if (!this->_hasBody)
				throw std::runtime_error("Func decl \"" + this->_idName + "\" must have a body!");

			if (this->_scannedMethod == nullptr)
				throw std::runtime_error("Critical error! Initial scan failed for method \"" + this->_idName + "\" of class \"" + currentClass->getClassName() + "\"!" + LINE_AND_FILE);
		}
		
		currentMethod = this->_scannedMethod;

		this->_body->fillTable(classTable, currentClass, currentMethod, initialScan);
	}
}

SemanticsBase* FuncDeclNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	if (this->_isOperatorOverload)
	{
		if (this->_overloadNotation == OverloadOperatorNotation::Postfix)
			throw std::runtime_error("Postfix notation is not supported for operator overload!" + LINE_AND_FILE);
		 
		//Must have a static modifier
		if (!this->_hasModifiers)
			throw std::runtime_error("Operator overloads must have static and public modifiers! All other modifiers are not supported! Mutating operators like \"+=\" are not supported!" + LINE_AND_FILE);

		//Check modifiers
		this->_modifiers = this->_modifiers->semanticsTransform(stack)->typecast<AccessModifierListNode>();

		bool hasStatic = false;
		bool hasPublic = false;
		bool hasAnythingOtherThanPublicOrStatic = false;
		for (auto& modifier : this->_modifiers->_vec)
		{
			if (modifier->_type == AccessModifierType::Static)
				hasStatic = true;
			else if (modifier->_type == AccessModifierType::Public)
				hasPublic = true;
			else
				hasAnythingOtherThanPublicOrStatic = true;
		}

		if (!hasStatic || !hasPublic || hasAnythingOtherThanPublicOrStatic)
			throw std::runtime_error("Operator overloads must have static and public modifiers! All other modifiers are not supported! Mutating operators like \"+=\" are not supported!" + LINE_AND_FILE);

		if (!this->_hasArgs)
			throw std::runtime_error("Operator overloads must have at least 1 argument!" + LINE_AND_FILE);

		//Check and enforce the correct operator count
		int argCount = this->_argList->_vec.size();

		bool isPrefixPostfix = this->_overloadNotation == OverloadOperatorNotation::Postfix || this->_overloadNotation == OverloadOperatorNotation::Prefix;
		if (isPrefixPostfix && argCount != 1)
			throw std::runtime_error("Operator overloads must have exactly 1 argument for prefix and postfix operators!" + LINE_AND_FILE);

		bool isInfix = this->_overloadNotation == OverloadOperatorNotation::Infix;
		if (isInfix && argCount != 2)
			throw std::runtime_error("Operator overloads must have exactly 2 arguments for infix operators!" + LINE_AND_FILE);

		if (this->_hasArgs)
		{
			this->_argList = this->_argList->semanticsTransform(stack)->typecast<FuncDeclArgListNode>();
		}
	}
	else
	{
		//Add default modifiers
		if (!this->_hasModifiers)
		{
			this->_modifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(AccessModifierType::Internal));
			this->_hasModifiers = true;
		}

		//Check modifiers
		this->_modifiers = this->_modifiers->semanticsTransform(stack)->typecast<AccessModifierListNode>();

		//Don't transform args of default main function
		if (this->_idName != RTLHelper::_defaultMainFunc)
		{
			if (this->_hasArgs)
			{
				this->_argList = this->_argList->semanticsTransform(stack)->typecast<FuncDeclArgListNode>();
			}
		}
	}

	//Transform return type
	if (this->_hasNonVoidReturn)
		this->_returnType = this->_returnType->semanticsTransform(stack)->typecast<TypeNode>();

	//Function may not have a body if it has a void return which is checked later
	if (this->_hasBody)
		this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();

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

}
