#pragma once
#include "dottable.h"

enum TypeType 
{
	BoolT,
	FloatT,
	IntT,
	StringT,
	CharacterT,
	IdT,
	ArrayT
};

class TypeNode : public Dottable
{
public:
	TypeType _type;

	std::string _idTypeName;

	TypeNode* _arrayType;

	static TypeNode* createType(TypeType type);

	static TypeNode* createIdType(std::string id);

	static TypeNode* createArrayType(TypeNode* type);

	std::string getName();

	void generateDot(std::ofstream& file) override;
};

