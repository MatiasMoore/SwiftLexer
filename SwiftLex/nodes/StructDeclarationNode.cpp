#include "StructDeclarationNode.h"
#include "AccessModifierNode.h"
#include "TypeForGenericNode.h"
#include "StmtNode.h"

StructDeclarationNode* StructDeclarationNode::createRegular(std::string name, StmtListNode* body)
{
    auto node = new StructDeclarationNode();
    node->_type = StructDeclarationType::NotGeneric;
    node->_name = name;
    node->_hasModifiers = false;

    if (body == nullptr)
    {
		node->_hasBody = false;
	}
    else
    {
		node->_hasBody = true;
		node->_body = body;
	}

    return node;
}

StructDeclarationNode* StructDeclarationNode::createGeneric(std::string name, TypeForGenericListNode* typesForGenericList, StmtListNode* body)
{
    auto node = StructDeclarationNode::createRegular(name, body);
    node->_type = StructDeclarationType::Generic;
    node->_typesForGenericList = typesForGenericList;
    return node;
}

StructDeclarationNode* StructDeclarationNode::addModifiers(AccessModifierListNode* modifiers)
{
    this->_hasModifiers = true;
    this->_modifiers = modifiers;
    return this;
}

void StructDeclarationNode::generateDot(std::ofstream& file)
{
    if (this->_type == StructDeclarationType::NotGeneric)
    {
        file << dotLabel(this->_id, "StructDeclaration: " + this->_name + " not generic");
    }
    else
    {
		file << dotLabel(this->_id, "StructDeclaration: " + this->_name + " generic");
        file << dotConnectionWithLabel(this->_id, this->_typesForGenericList->_id, "generic types");
        this->_typesForGenericList->generateDot(file);
	}

    if (this->_hasModifiers)
    {
		file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
		this->_modifiers->generateDot(file);
	}

    if (this->_hasBody)
    {
		file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
		this->_body->generateDot(file);
	}
}
