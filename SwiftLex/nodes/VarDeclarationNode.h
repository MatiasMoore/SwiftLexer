#pragma once
#include "dottable.h"
#include "GenericListNode.h"

class ExprNode;
class TypeNode;
class AccessModifierListNode;

enum VarDeclType
{
	ValueKnown,
	TypeKnown,
	ValueAndTypeKnown
};

class StringList
{
public:
	std::vector<std::string> _vec = {};

	static StringList* createList(std::string str)
	{
		auto list = new StringList();
		list->_vec.push_back(str);
		return list;
	}

	StringList* appendElem(std::string str)
	{
		_vec.push_back(str);
		return this;
	}
};

//Forward declaration for list
class VarDeclarationNode;

class VarDeclarationListNode : public GenericListNode<VarDeclarationNode, VarDeclarationListNode>
{
public:
	std::string getName() override;

	VarDeclarationListNode* addModifiers(AccessModifierListNode* modifiers);
};

class VarDeclarationNode : public Dottable
{
public:
	VarDeclType _type;
	std::string _varName;

	ExprNode* _valueNode;
	TypeNode* _typeNode;

	AccessModifierListNode* _modifiers;
	bool _hasModifiers = false;

	static VarDeclarationNode* createFromValue(std::string varName, ExprNode* value);

	static VarDeclarationNode* createFromType(std::string varName, TypeNode* type);

	static VarDeclarationNode* createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type);

	VarDeclarationNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

