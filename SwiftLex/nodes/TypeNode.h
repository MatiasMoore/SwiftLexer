#pragma once
#include "dottable.h"
#include "../tables/tables.h"

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
	DynamicT
};

class TypeNode : public Dottable
{
public:
	TypeType _type;

	std::string _idTypeName;

	TypeNode* _arrayType;

	ExprNode* _exprForDynamicT;

	static TypeNode* createType(TypeType type);

	static TypeNode* createIdType(std::string id);

	static TypeNode* createArrayType(TypeNode* type);

	static TypeNode* createDynamicType(ExprNode* expr);

	static TypeNode* createFromDescriptor(std::string descriptor);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	std::string toDescriptor(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);
};

