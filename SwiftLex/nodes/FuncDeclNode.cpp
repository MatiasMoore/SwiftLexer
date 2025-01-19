#include "FuncDeclNode.h"
#include "StmtNode.h"
#include "FuncDeclArgNode.h"
#include "TypeNode.h"
#include "AccessModifierNode.h"
#include "ReturnNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"

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

FuncDeclNode* FuncDeclNode::createRegularOperator(OverloadableOperatorType overloadType, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isOperatorOverload = true;
	node->_overloadOpearatorType = overloadType;
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

std::string FuncDeclNode::getOperatorOverloadName(OverloadableOperatorType overloadType)
{
	switch (this->_overloadOpearatorType)
	{
		case (OverloadableOperatorType::OpPLUS):
			return "OpPLUS";
				break;
		case (OverloadableOperatorType::OpMINUS):
			return "OpMINUS";
				break;
		case (OverloadableOperatorType::OpMUL):
			return "OpMUL";
				break;
		case (OverloadableOperatorType::OpDIV):
			return "OpDIV";
				break;
		case (OverloadableOperatorType::OpMOD):
			return "OpMOD";
				break;
		case (OverloadableOperatorType::OpLT):
			return "OpLT";
				break;
		case (OverloadableOperatorType::OpGT):
			return "OpGT";
				break;
		case (OverloadableOperatorType::OpGTE):
			return "OpGTE";
				break;
		case (OverloadableOperatorType::OpLTE):
			return "OpLTE";
				break;
		case (OverloadableOperatorType::OpEQ):
			return "OpEQ";
				break;
		case (OverloadableOperatorType::OpNEQ):
			return "OpNEQ";
				break;
		case (OverloadableOperatorType::OpBitAND):
			return "OpBitAND";
				break;
		case (OverloadableOperatorType::OpBitOR):
			return "OpBitOR";
				break;
		case (OverloadableOperatorType::OpBitXOR):
			return "OpBitXOR";
				break;
		case (OverloadableOperatorType::OpLogAND):
			return "OpLogAND";
				break;
		case (OverloadableOperatorType::OpLogOR):
			return "OpLogOR";
				break;
		case (OverloadableOperatorType::OpLSHIFT):
			return "OpLSHIFT";
				break;
		case (OverloadableOperatorType::OpRSHIFT):
			return "OpRSHIFT";
				break;
		case (OverloadableOperatorType::OpCLOSEDRANGE):
			return "OpCLOSEDRANGE";
				break;
		case (OverloadableOperatorType::OpHALFOPENRANGE):
			return "OpHALFOPENRANGE";
				break;
		case (OverloadableOperatorType::OpNILCOALESCE):
			return "OpNILCOALESC";
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
		file << dotLabel(this->_id, "OperatorOverloadDecl\noperatorName: " + this->getOperatorOverloadName(this->_overloadOpearatorType) + extraInfo);
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

void FuncDeclNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Func decl must be associated with a class!");

	if (currentMethod != nullptr)
		throw std::runtime_error("Func decl can't be inside a method!");

	std::string strDesc = "(";

	if (this->_hasArgs)
	{
		for (auto& arg : this->_argList->_vec)
		{
			strDesc += arg->_argType->toDescriptor(classTable, currentClass, currentMethod);
		}
	}
	strDesc += ")";

	if (this->_hasNonVoidReturn)
		strDesc += this->_returnType->toDescriptor(classTable, currentClass, currentMethod);
	else
		strDesc += "V";

	if (!this->_hasModifiers)
		throw std::runtime_error("Func decl \"" + this->_idName + "\" must have access modifiers!");

	auto accessFlags = this->_modifiers->getMethodAccessFlags();
	currentMethod = currentClass->addMethod(this->_idName, this->_body, strDesc, accessFlags);

	bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();

	if (!isStatic)
	{
		currentMethod->varTable->addLocalVarToConstantTable("self", TypeNode::createIdType(currentClass->nameStr)->toDescriptor(classTable, currentClass, currentMethod), currentClass->constants);
	}

	if (this->_hasArgs)
	{
		for (auto& arg : this->_argList->_vec)
		{
			currentMethod->varTable->addLocalVarToConstantTable(arg->_argName, arg->_argType->toDescriptor(classTable, currentClass, currentMethod), currentClass->constants);
		}
	}

	if (!this->_hasBody)
		throw std::runtime_error("Func decl \"" + this->_idName + "\" must have a body!");

	this->_body->fillTable(classTable, currentClass, currentMethod);
}

SemanticsBase* FuncDeclNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);

	//Add default modifiers
	if (!this->_hasModifiers)
	{
		this->_modifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(AccessModifierType::Internal));
		this->_hasModifiers = true;
	}

	//Check modifiers
	this->_modifiers = this->_modifiers->semanticsTransform(stack)->typecast<AccessModifierListNode>();

	//Function may not have a body if it has a void return which is checked later
	if (this->_hasBody)
		this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();

	//Check if function ends with a return
	bool hasReturnStmt = this->_hasBody 
		&& this->_body->_vec.back()->_type == StmtType::Return; //last stmt is a return

	if (!hasReturnStmt)
	{
		//Function has a return type and has no return stmt
		if (this->_hasNonVoidReturn)
			throw std::runtime_error("Method \"" + this->_idName + "\" must have at least a return stmt inside the body!");
		
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
