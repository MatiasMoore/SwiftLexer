#pragma once
#include "dottable.h"

class StmtListNode;
class FuncDeclArgListNode;
class TypeForGenericListNode;
class TypeNode;
class AccessModifierListNode;

enum FuncDeclType
{
	generic,
	notGeneric
};

class FuncDeclNode : public Dottable
{
public:
	FuncDeclType _type;

	bool _hasModifiers;
	bool _hasArgs;
	bool _hasBody;
	bool _hasNonVoidReturn;
	bool _throwsException;

	AccessModifierListNode* _modifiers;
	std::string _idName;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;
	TypeNode* _returnType;

	TypeForGenericListNode* _typesForGenericList;

	static FuncDeclNode* createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createGeneric(std::string idName, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	FuncDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;
};

