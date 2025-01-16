#include "AccessModifierNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"

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

std::vector<enum MethodAccessFlag> AccessModifierNode::getAccessFlags()
{
	switch (this->_type)
	{
	case (AccessModifierType::Static):
		return { M_ACC_STATIC };
		break;
	case (AccessModifierType::Final):
		return { M_ACC_FINAL };
		break;
	case (AccessModifierType::Public):
		return { M_ACC_PUBLIC };
		break;
	case (AccessModifierType::Private):
		return { M_ACC_PRIVATE };
		break;
	default:
		throw std::runtime_error("Access flag node with type " + std::to_string(this->_type) + " can't convert to jvm access flag!");
		break;
	}
}

std::string AccessModifierListNode::getName()
{
	return "AccessModifierList";
}

std::vector<enum MethodAccessFlag> AccessModifierListNode::getAccessFlags()
{
	std::vector<enum MethodAccessFlag> flags = {};
	for (auto& flagNode : _vec)
	{
		auto flagsForNode =  flagNode->getAccessFlags();
		for (auto& flag : flagsForNode)
			flags.push_back(flag);
	}
	return flags;
}
