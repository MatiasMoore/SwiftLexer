#pragma once
#include "dottable.h"
#include "../tables/tables.h"
#include "SemanticsBase.h"

class ExprNode;

enum TypeType 
{
	BoolT,
	FloatT,
	IntT,
	StringT,
	CharacterT,
	IdT,
	ArrayT,
};

class TypeNode : public Dottable, public SemanticsBase
{
public:
	TypeType _type;

	std::string _idTypeName;

	TypeNode* _arrayType;

	static TypeNode* createType(TypeType type);

	static TypeNode* createIdType(std::string id);

	static TypeNode* createArrayType(TypeNode* type);

	static TypeNode* createFromDescriptor(std::string descriptor);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	std::string toDescriptor();
};

