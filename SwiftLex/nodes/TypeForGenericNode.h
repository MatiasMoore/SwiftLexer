#pragma once
#include "dottable.h"
#include "GenericListNode.h"

enum TypeForGenericType
{
	idOnly,
	idWithBaseClass
};

class TypeForGenericNode : public Dottable
{
public:
	TypeForGenericType _type;

	std::string _idName;
	std::string _baseClassName;

	static TypeForGenericNode* createNoBaseClass(std::string idName);

	static TypeForGenericNode* createWithBaseClass(std::string idName, std::string baseClassName);

	void generateDot(std::ofstream& file) override;
};

class TypeForGenericListNode : public GenericListNode<TypeForGenericNode, TypeForGenericListNode>
{
public:
	std::string getName() override;
};