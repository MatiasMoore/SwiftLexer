#include "FuncDeclNode.h"
#include "StmtNode.h"
#include "FuncDeclArgNode.h"
#include "TypeForGenericNode.h"
#include "TypeNode.h"
#include "AccessModifierNode.h"

FuncDeclNode* FuncDeclNode::createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isGeneric = false;
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

FuncDeclNode* FuncDeclNode::createGeneric(std::string idName, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isGeneric = true;
	node->_isOperatorOverload = false;
	node->_idName = idName;
	node->_typesForGenericList = typesForGenericList;
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
	node->_isGeneric = false;
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

FuncDeclNode* FuncDeclNode::createGenericOperator(OverloadableOperatorType overloadType, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_isGeneric = true;
	node->_isOperatorOverload = true;
	node->_overloadOpearatorType = overloadType;
	node->_typesForGenericList = typesForGenericList;
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
		if (!this->_isGeneric)
		{
			file << dotLabel(this->_id, "OperatorOverloadDecl\noperatorName: " + this->getOperatorOverloadName(this->_overloadOpearatorType) + "\nnot generic" + extraInfo);
		}
		else
		{
			file << dotLabel(this->_id, "OperatorOverloadDecl\noperatorName: " + this->getOperatorOverloadName(this->_overloadOpearatorType) + "\ngeneric" + extraInfo);
			file << dotConnectionWithLabel(this->_id, this->_typesForGenericList->_id, "generic types");
			this->_typesForGenericList->generateDot(file);
		}
	}
	else
	{
		if (!this->_isGeneric)
		{
			file << dotLabel(this->_id, "FuncDecl\nidName: " + this->_idName + "\nnot generic" + extraInfo);
		}
		else
		{
			file << dotLabel(this->_id, "FuncDecl\nidName: " + this->_idName + "\ngeneric" + extraInfo);
			file << dotConnectionWithLabel(this->_id, this->_typesForGenericList->_id, "generic types");
			this->_typesForGenericList->generateDot(file);
		}
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
