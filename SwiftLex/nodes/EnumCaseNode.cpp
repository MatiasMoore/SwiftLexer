#include "EnumCaseNode.h"
#include "IdListNode.h"

EnumCaseNode* EnumCaseNode::createEnumCaseNode(IdListNode* idList)
{
	auto node = new EnumCaseNode();
	node->_idList = idList;
	return node;
}

void EnumCaseNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "EnumCase");
	file << dotConnection(this->_id, this->_idList->_id);
	this->_idList->generateDot(file);
}
