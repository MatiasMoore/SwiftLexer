#pragma once
#include "dottable.h"
class StmtListNode;
class AccessModifierListNode;

class EnumDeclarationNode : public Dottable
{
public:
	StmtListNode* _stmtList;

	std::string _name;

	AccessModifierListNode* _modifiers;

	static EnumDeclarationNode* createEnumDeclarationNode(std::string name, StmtListNode* stmtList);

	EnumDeclarationNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

