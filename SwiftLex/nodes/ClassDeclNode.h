#pragma once
#include "dottable.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

class StmtListNode;
class TypeForGenericListNode;
class AccessModifierListNode;

enum ClassDeclType
{
	NoBaseClass,
	HasBaseClass
};

class ClassDeclNode : public Dottable, public SemanticsBase
{
public:
	ClassDeclType _type;

	std::string _name;
	std::string _baseClassName;

	StmtListNode* _body;
	bool _hasBody;

	AccessModifierListNode* _modifiers;
	bool _hasModifiers;

	InternalClass* _scannedClass;

	static ClassDeclNode* createClass(std::string name, StmtListNode* body);

	static ClassDeclNode* createClassWithBaseClass(std::string name, StmtListNode* body, std::string baseClass);

	ClassDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file);

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan = false);

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

