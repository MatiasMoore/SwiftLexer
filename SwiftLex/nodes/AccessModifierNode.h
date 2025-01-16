#pragma once
#include "dottable.h"
#include "GenericListNode.h"

enum AccessModifierType
{
	Static,
	Final,
	Override,
	Open,
	Public,
	Internal,
	Fileprivate,
	Private,
	Prefix,
	Postfix
};

class AccessModifierNode : public Dottable
{
public:
	AccessModifierType _type;

	static AccessModifierNode* createModifier(AccessModifierType type);

	void generateDot(std::ofstream& file);

	std::vector<enum MethodAccessFlag> getAccessFlags();
};

class AccessModifierListNode : public GenericListNode<AccessModifierNode, AccessModifierListNode>
{
public:
	std::string getName();

	std::vector<enum MethodAccessFlag> getAccessFlags();
};

