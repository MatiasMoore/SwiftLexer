#include "DestructorDeclNode.h"
#include "StmtNode.h"

DestructorDeclNode* DestructorDeclNode::createDestructor(StmtListNode* body)
{
	auto node = new DestructorDeclNode();
	node->_body = body;
	return node;
}

void DestructorDeclNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "DestructorDecl");
	this->_body->generateDot(file);
}
