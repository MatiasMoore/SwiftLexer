#include "IdListNode.h"

std::string IdListNode::getName()
{
	return "IdList";
}

IdNode* IdNode::create(std::string idName)
{
	auto node = new IdNode();
	node->_idName = idName;
	return node;
}

void IdNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "ID: " + this->_idName);
}
