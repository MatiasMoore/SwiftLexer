#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

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
};

class AccessModifierNode : public Dottable, public SemanticsBase
{
public:
	AccessModifierType _type;

	static AccessModifierNode* createModifier(AccessModifierType type);

	std::string getName();

	void generateDot(std::ofstream& file);

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
	
	std::vector<MethodAccessFlag> getMethodAccessFlags();

	std::vector<FieldAccessFlag> getFieldAccessFlags();
};

class AccessModifierListNode : public GenericListNode<AccessModifierNode, AccessModifierListNode>, public SemanticsBase
{
public:
	std::string getName();

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	std::vector<MethodAccessFlag> getMethodAccessFlags();

	std::vector<FieldAccessFlag> getFieldAccessFlags();
};

