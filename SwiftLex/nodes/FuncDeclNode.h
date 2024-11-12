#pragma once
#include "dottable.h"

class StmtListNode;
class FuncDeclArgListNode;
class TypeForGenericListNode;
class TypeNode;

enum FuncDeclType
{
	generic,
	notGeneric
};

class FuncDeclNode : public Dottable
{
public:
	FuncDeclType _type;

	bool _hasArgs;
	bool _hasBody;
	bool _hasNonVoidReturn;
	bool _throwsException;

	std::string _idName;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;
	TypeNode* _returnType;

	TypeForGenericListNode* _typesForGenericList;

	static FuncDeclNode* createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createGeneric(std::string idName, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	void generateDot(std::ofstream& file) override;
};

