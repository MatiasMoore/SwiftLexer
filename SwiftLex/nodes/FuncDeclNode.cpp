#include "FuncDeclNode.h"
#include "StmtNode.h"

FuncDeclNode* FuncDeclNode::createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException)
{
	auto node = new FuncDeclNode();
	node->_type = FuncDeclType::notGeneric;
	node->_idName = idName;

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
	node->_type = FuncDeclType::generic;
	node->_idName = idName;
	node->_typesForGenericList = typesForGenericList;

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

void FuncDeclNode::generateDot(std::ofstream& file)
{
	std::string extraInfo;
	if (!this->_hasArgs)
		extraInfo += "\nno args";
	if (!this->_hasBody)
		extraInfo += "\nno body";
	if (!this->_hasNonVoidReturn)
		extraInfo += "\nvoid return";
	
	extraInfo += (std::string)(this->_throwsException ? "\nthrows exception" : "\ndoesn't throw exception");

	if (this->_type == FuncDeclType::notGeneric)
	{
		file << dotLabel(this->_id, "FuncDecl\nidName: " + this->_idName + "\nnot generic" + extraInfo);
	}
	else
	{
		file << dotLabel(this->_id, "FuncDecl\nidName: " + this->_idName + "\ngeneric" + extraInfo);
		file << dotConnectionWithLabel(this->_id, this->_typesForGenericList->_id, "generic types");
		this->_typesForGenericList->generateDot(file);
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
