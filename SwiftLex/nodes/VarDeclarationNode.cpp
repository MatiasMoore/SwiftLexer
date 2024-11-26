#include "VarDeclarationNode.h"
#include "TypeNode.h"
#include "ExprNode.h"
#include "AccessModifierNode.h"

VarDeclarationNode* VarDeclarationNode::createFromValue(std::string varName, ExprNode* value)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueKnown;
	node->_valueNode = value;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromType(std::string varName, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = TypeKnown;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueAndTypeKnown;
	node->_valueNode = value;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_hasModifiers = true;
	this->_modifiers = modifiers;
	return this;
}

void VarDeclarationNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "VarDecl: " + _varName);
	if (this->_hasModifiers)
	{
		file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
		this->_modifiers->generateDot(file);
	}
	if (this->_type == VarDeclType::TypeKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_typeNode->_id, "type");
		this->_typeNode->generateDot(file);
	}
	if (this->_type == VarDeclType::ValueKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_valueNode->_id, "value");
		this->_valueNode->generateDot(file);
	}
}

std::string VarDeclarationListNode::getName()
{
	return "VarDeclList";
}

VarDeclarationListNode* VarDeclarationListNode::addModifiers(AccessModifierListNode* modifiers)
{
	for (auto& varDecl : this->_vec)
	{
		varDecl->addModifiers(modifiers);
	}
	return this;
}
