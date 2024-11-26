#include "EnumDeclarationNode.h"
#include "StmtNode.h"
#include "AccessModifierNode.h"


EnumDeclarationNode* EnumDeclarationNode::createEnumDeclarationNode(std::string name, StmtListNode* stmtList)
{
	auto node = new EnumDeclarationNode();
	node->_stmtList = stmtList;
	node->_name = name;
	return node;
}

EnumDeclarationNode* EnumDeclarationNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_modifiers = modifiers;
	return this;
}

void EnumDeclarationNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "EnumDeclaration: " + this->_name);

	if (this->_modifiers != nullptr) {
		file << dotConnection(this->_id, this->_modifiers->_id);
		this->_modifiers->generateDot(file);
	}

	if (this->_stmtList != nullptr) {
		file << dotConnection(this->_id, this->_stmtList->_id);
		this->_stmtList->generateDot(file);
	}
}
