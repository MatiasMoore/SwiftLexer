#pragma once
#include "dottable.h"

class FuncDeclArgListNode;
class StmtListNode;
class AccessModifierListNode;
class TypeNode;
class TypeForGenericListNode;

class ConstructorDeclNode : public Dottable
{
public:
	bool _hasModifiers;
	bool _hasArgs;
	bool _hasBody;
	bool _throwsException;

	AccessModifierListNode* _modifiers;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;

	static ConstructorDeclNode* createConstructor(FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException);

	ConstructorDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);
};

