#pragma once
#include "dottable.h"
#include "../tables/tables.h"
#include "SemanticsBase.h"

class FuncDeclArgListNode;
class StmtListNode;
class AccessModifierListNode;
class TypeNode;
class TypeForGenericListNode;

class ConstructorDeclNode : public Dottable, public SemanticsBase
{
public:
	bool _hasModifiers;
	bool _hasArgs;
	bool _hasBody;
	bool _throwsException;

	AccessModifierListNode* _modifiers;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;

	InternalMethod* _scannedConstructor;
	VariableScope* _scannedScope;

	static ConstructorDeclNode* createConstructor(FuncDeclArgListNode* argList, StmtListNode* body, bool throwsException);

	ConstructorDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan = false);
};

