#include "TypeNode.h"

TypeNode* TypeNode::createType(TypeType type)
{
	auto node = new TypeNode();
	node->_type = type;
	return node;
}

TypeNode* TypeNode::createArrayType(TypeNode* type)
{
	auto node = new TypeNode();
	node->_type = ArrayT;
	node->_arrayType = type;
	return node;
}
