#pragma once
#include "dottable.h"

class AccessModifierListNode;
class TypeForGenericListNode;
class StmtListNode;

enum StructDeclarationType
{
	NotGeneric,
	Generic
};

class StructDeclarationNode : public Dottable
{
public:
	StructDeclarationType _type;

	bool _hasBody;
	bool _hasModifiers;
	
	std::string _name;
	AccessModifierListNode* _modifiers;
	StmtListNode* _body;

	TypeForGenericListNode* _typesForGenericList;

	static StructDeclarationNode* createRegular(std::string name, StmtListNode* body);

	static StructDeclarationNode* createGeneric(std::string name, TypeForGenericListNode* typesForGenericList, StmtListNode* body);

	StructDeclarationNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

