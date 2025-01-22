#include "AccessModifierNode.h"
#include "../generation/generationHelpers.h"
#include <set>

AccessModifierNode* AccessModifierNode::createModifier(AccessModifierType type)
{
	auto node = new AccessModifierNode();
	node->_type = type;
	return node;
}

std::string AccessModifierNode::getName()
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
	return typeName;
}

void AccessModifierNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, this->getName());
}

std::vector<enum MethodAccessFlag> AccessModifierNode::getMethodAccessFlags()
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
	case (AccessModifierType::Internal):
		return { M_ACC_PRIVATE };
		break;
	case (AccessModifierType::Override):
		//All java methods override by default, we set a custom flag
		return { M_ACC_CUSTOM_OVERRIDE };
		break;
	default:
		throw std::runtime_error("Method access flag node with type " + std::to_string(this->_type) + " can't convert to jvm access flag!");
		break;
	}
}

std::vector<enum FieldAccessFlag> AccessModifierNode::getFieldAccessFlags()
{
	switch (this->_type)
	{
	case (AccessModifierType::Static):
		return { F_ACC_STATIC };
		break;
	case (AccessModifierType::Final):
		return { F_ACC_FINAL };
		break;
	case (AccessModifierType::Public):
		return { F_ACC_PUBLIC };
		break;
	case (AccessModifierType::Private):
		return { F_ACC_PRIVATE };
		break;
	default:
		throw std::runtime_error("Field access flag node with type " + std::to_string(this->_type) + " can't convert to jvm access flag!");
		break;
	}
}

SemanticsBase* AccessModifierNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	auto myModifierList = stack.getClosest<AccessModifierListNode>();
	if (myModifierList == nullptr)
		throw std::runtime_error("Critical error! Can't find parent modifier list for modifier!");

	std::set<AccessModifierType> conflictingTypes = {};

	switch (this->_type)
	{
	case Static:
		conflictingTypes = { Override, Open };
		break;
	case Final:
		conflictingTypes = { Override, Open };
		break;
	case Override:
		conflictingTypes = { Static, Final};
		break;
	case Open:
		conflictingTypes = { Final, Static};
		break;
	case Private:
		conflictingTypes = { Fileprivate, Public, Internal };
		break;
	case Fileprivate:
		conflictingTypes = { Private, Public, Internal };
		break;
	case Internal:
		conflictingTypes = { Fileprivate, Public, Private };
		break;
	case Public:
		conflictingTypes = { Private, Internal, Fileprivate };
		break;
	default:
		throw std::runtime_error("Access modifier with enum type " + std::to_string(this->_type) + " is not supported!");
	}

	for (auto& modifier : myModifierList->_vec)
	{
		if (conflictingTypes.find(modifier->_type) != conflictingTypes.end())
			throw std::runtime_error("Access modifier \"" + this->getName() + "\" conflicts with access modifier \"" + modifier->getName() + "\"!");

		bool sameType = modifier->_type == this->_type;
		bool isThisNode = modifier == this;
		if (sameType && !isThisNode)
			throw std::runtime_error("Access modifier \"" + this->getName() + "\" is used more than once!");
	}

	return this;
}

std::string AccessModifierListNode::getName()
{
	return "AccessModifierList";
}

std::vector<enum MethodAccessFlag> AccessModifierListNode::getMethodAccessFlags()
{
	std::vector<enum MethodAccessFlag> flags = {};
	for (auto& flagNode : _vec)
	{
		auto flagsForNode =  flagNode->getMethodAccessFlags();
		for (auto& flag : flagsForNode)
			flags.push_back(flag);
	}
	return flags;
}

std::vector<enum FieldAccessFlag> AccessModifierListNode::getFieldAccessFlags()
{
	std::vector<enum FieldAccessFlag> flags = {};
	this;
	if (_vec.size() == 0)
	{
		throw std::runtime_error("No access modifier");
	}
	for (auto& flagNode : _vec)
	{
		auto flagsForNode = flagNode->getFieldAccessFlags();
		for (auto& flag : flagsForNode)
			flags.push_back(flag);
	}
	return flags;
}

SemanticsBase* AccessModifierListNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	for (int i = 0; i < _vec.size(); i++)
	{
		_vec[i] = _vec[i]->semanticsTransform(stack)->typecast<AccessModifierNode>();
	}
	return this;
}
