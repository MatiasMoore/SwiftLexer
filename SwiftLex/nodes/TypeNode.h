#pragma once

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

class TypeNode
{
public:
	TypeType _type;

	TypeNode* _arrayType;

	static TypeNode* createType(TypeType type);

	static TypeNode* createArrayType(TypeNode* type);

};

