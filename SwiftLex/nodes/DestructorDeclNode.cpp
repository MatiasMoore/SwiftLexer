#include "DestructorDeclNode.h"
#include "StmtNode.h"

DestructorDeclNode* DestructorDeclNode::createDestructor(StmtListNode* body)
{
	auto node = new DestructorDeclNode();
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

void DestructorDeclNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "DestructorDecl" + std::string(!this->_hasBody ? "\nno body" : ""));
	if (this->_hasBody)
	{
		file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
		this->_body->generateDot(file);
	}
}
