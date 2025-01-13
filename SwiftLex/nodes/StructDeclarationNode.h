#pragma once
#include "dottable.h"

class AccessModifierListNode;
class TypeForGenericListNode;
class StmtListNode;

class StructDeclarationNode : public Dottable
{
public:
	bool _hasBody;
	bool _hasModifiers;
	
	std::string _name;
	AccessModifierListNode* _modifiers;
	StmtListNode* _body;

	static StructDeclarationNode* createRegular(std::string name, StmtListNode* body);

	StructDeclarationNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

