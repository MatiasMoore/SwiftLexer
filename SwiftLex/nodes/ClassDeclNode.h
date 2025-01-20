#pragma once
#include "dottable.h"
#include "SemanticsBase.h"

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

	static ClassDeclNode* createClass(std::string name, StmtListNode* body);

	static ClassDeclNode* createClassWithBaseClass(std::string name, StmtListNode* body, std::string baseClass);

	ClassDeclNode* addModifiers(AccessModifierListNode* modifiers);

	void generateDot(std::ofstream& file);

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

