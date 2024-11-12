#include "TypeForGenericNode.h"

TypeForGenericNode* TypeForGenericNode::createNoBaseClass(std::string idName)
{
	auto node = new TypeForGenericNode();
	node->_type = TypeForGenericType::idOnly;
	node->_idName = idName;
	return node;
}

TypeForGenericNode* TypeForGenericNode::createWithBaseClass(std::string idName, std::string baseClassName)
{
	auto node = new TypeForGenericNode();
	node->_type = TypeForGenericType::idWithBaseClass;
	node->_idName = idName;
	node->_baseClassName = baseClassName;
	return node;
}

void TypeForGenericNode::generateDot(std::ofstream& file)
{
	if (_type == TypeForGenericType::idOnly)
	{ 
		file << dotLabel(this->_id, "GenericType\nidName: " + this->_idName);
	}
	else
	{
		file << dotLabel(this->_id, "GenericType\nidName: " + this->_idName + "\nbaseClassName: " + this->_baseClassName);
	}
}

std::string TypeForGenericListNode::getName()
{
	return "TypeForGenericList";
}
