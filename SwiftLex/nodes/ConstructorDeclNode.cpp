#include "ConstructorDeclNode.h"
#include "FuncDeclArgNode.h"
#include "StmtNode.h"
#include "AccessModifierNode.h"
#include "TypeForGenericNode.h"

ConstructorDeclNode* ConstructorDeclNode::createConstructor(FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException)
{
    auto node = new ConstructorDeclNode();

	node->_type = ConstructorType::notGenericType;
	
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

ConstructorDeclNode* ConstructorDeclNode::createConstructorGeneric(TypeForGenericListNode* genericList, FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException)
{
	auto node = createConstructor(argList, body, throwsException);
	node->_type = ConstructorType::genericType;
	node->_genericList = genericList;
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

	if (this->_type == ConstructorType::genericType)
	{
		file << dotConnectionWithLabel(this->_id, this->_genericList->_id, "generic types");
		this->_genericList->generateDot(file);
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
}
