#include "EnumDeclarationNode.h"
#include "StmtNode.h"


EnumDeclarationNode* EnumDeclarationNode::createEnumDeclarationNode(std::string name, StmtListNode* stmtList)
{
	auto node = new EnumDeclarationNode();
	node->_stmtList = stmtList;
	node->_name = name;
	return node;
}

void EnumDeclarationNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "EnumDeclaration: " + this->_name);
	file << dotConnection(this->_id, this->_stmtList->_id);
	this->_stmtList->generateDot(file);
}
