#pragma once
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

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

class VarDeclarationListNode : public GenericListNode<VarDeclarationNode, VarDeclarationListNode>, public SemanticsBase
{
public:
	std::string getName() override;

	VarDeclarationListNode* addModifiers(AccessModifierListNode* modifiers);

	VarDeclarationListNode* setAsFieldDecl(bool flag);

	bool isFieldDecl();

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

class VarDeclarationNode : public Dottable, public SemanticsBase
{
public:
	VarDeclType _type;
	std::string _varName;

	ExprNode* _valueNode;
	TypeNode* _typeNode;

	AccessModifierListNode* _modifiers;
	bool _hasModifiers = false;
	bool _isFieldDecl = false;

	static VarDeclarationNode* createFromValue(std::string varName, ExprNode* value);

	static VarDeclarationNode* createFromType(std::string varName, TypeNode* type);

	static VarDeclarationNode* createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type);

	VarDeclarationNode* addModifiers(AccessModifierListNode* modifiers);

	VarDeclarationNode* setAsFieldDecl(bool flag);

	void generateDot(std::ofstream& file) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
	
	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

};

