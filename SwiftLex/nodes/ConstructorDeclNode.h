#pragma once
#include "dottable.h"

class FuncDeclArgListNode;
class StmtListNode;
class AccessModifierListNode;
class TypeNode;
class TypeForGenericListNode;

enum ConstructorType
{
	notGenericType,
	genericType
};

class ConstructorDeclNode : public Dottable
{
public:
	ConstructorType _type;

	bool _hasModifiers;
	bool _hasArgs;
	bool _hasBody;
	bool _throwsException;

	AccessModifierListNode* _modifiers;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;

	TypeForGenericListNode* _genericList;

	static ConstructorDeclNode* createConstructor(FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException);

	static ConstructorDeclNode* createConstructorGeneric(TypeForGenericListNode* genericList, FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException);

	ConstructorDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

