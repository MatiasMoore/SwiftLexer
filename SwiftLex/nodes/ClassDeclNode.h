#pragma once
#include "dottable.h"

class StmtListNode;
class TypeForGenericListNode;
class AccessModifierListNode;

enum ClassDeclType
{
	NoBaseClass,
	HasBaseClass
};

class ClassDeclNode : public Dottable
{
public:
	ClassDeclType _type;

	std::string _name;
	std::string _baseClassName;

	StmtListNode* _body;
	bool _hasBody;

	TypeForGenericListNode* _typesForGenericList;
	bool _isGeneric;

	AccessModifierListNode* _modifiers;
	bool _hasModifiers;

	static ClassDeclNode* createClass(std::string name, StmtListNode* body, TypeForGenericListNode* genericTypes);

	static ClassDeclNode* createClassWithBaseClass(std::string name, StmtListNode* body, TypeForGenericListNode* genericTypes, std::string baseClass);

	ClassDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file);
};

