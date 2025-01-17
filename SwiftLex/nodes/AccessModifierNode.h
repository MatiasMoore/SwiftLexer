#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"

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

class AccessModifierNode : public Dottable, public SemanticsBase
{
public:
	AccessModifierType _type;

	static AccessModifierNode* createModifier(AccessModifierType type);

	std::string getName();

	void generateDot(std::ofstream& file);

	std::vector<enum MethodAccessFlag> getAccessFlags();

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

class AccessModifierListNode : public GenericListNode<AccessModifierNode, AccessModifierListNode>, public SemanticsBase
{
public:
	std::string getName();

	std::vector<enum MethodAccessFlag> getAccessFlags();

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

