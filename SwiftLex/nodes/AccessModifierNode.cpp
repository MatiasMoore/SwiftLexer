#include "AccessModifierNode.h"

AccessModifierNode* AccessModifierNode::createModifier(AccessModifierType type)
{
	auto node = new AccessModifierNode();
	node->_type = type;
	return node;
}

void AccessModifierNode::generateDot(std::ofstream& file)
{
	std::string typeName = "";
	switch (this->_type)
	{
	case (AccessModifierType::Static):
		typeName = "Static";
		break;
	case (AccessModifierType::Final):
		typeName = "Final";
		break;
	case (AccessModifierType::Override):
		typeName = "Override";
		break;
	case (AccessModifierType::Open):
		typeName = "Open";
		break;
	case (AccessModifierType::Public):
		typeName = "Public";
		break;
	case (AccessModifierType::Internal):
		typeName = "Internal";
		break;
	case (AccessModifierType::Fileprivate):
		typeName = "Fileprivate";
		break;
	case (AccessModifierType::Private):
		typeName = "Private";
		break;
	case (AccessModifierType::Prefix):
		typeName = "Prefix";
		break;
	case (AccessModifierType::Postfix):
		typeName = "Postfix";
		break;
	default:
		throw new std::runtime_error("Unknown type!");
		break;
	}
	file << dotLabel(this->_id, typeName);
}

std::string AccessModifierListNode::getName()
{
	return "AccessModifierList";
}
