#pragma once
#include "dottable.h"
#include "ExprNode.h"
#include "TypeNode.h"
#include "GenericListNode.h"

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
};

class VarDeclarationNode : public Dottable
{
public:
	VarDeclType _type;
	std::string _varName;

	ExprNode* _valueNode;
	TypeNode* _typeNode;

	static VarDeclarationNode* createFromValue(std::string varName, ExprNode* value);

	static VarDeclarationNode* createFromType(std::string varName, TypeNode* type);

	static VarDeclarationNode* createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type);

	void generateDot(std::ofstream& file) override;
};

