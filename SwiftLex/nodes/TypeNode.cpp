#include "TypeNode.h"

TypeNode* TypeNode::createType(TypeType type)
{
	auto node = new TypeNode();
	node->_type = type;
	return node;
}

TypeNode* TypeNode::createIdType(std::string id)
{
	auto node = new TypeNode();
	node->_type = IdT;
	node->_idTypeName = id;
	return node;
}

TypeNode* TypeNode::createArrayType(TypeNode* type)
{
	auto node = new TypeNode();
	node->_type = ArrayT;
	node->_arrayType = type;
	return node;
}

std::string TypeNode::getName()
{
	switch (this->_type)
	{
		case (TypeType::BoolT):
			return "Bool";
			break;
		case (TypeType::FloatT):
			return "Float";
			break;
		case (TypeType::IntT):
			return "Int";
			break;
		case (TypeType::StringT):
			return "String";
			break;
		case (TypeType::CharacterT):
			return "Character";
			break;
		case (TypeType::IdT):
			return "Id: " + this->_idTypeName;
			break;
		case (TypeType::ArrayT):
			return "Array";
			break;
	}
}

void TypeNode::generateDot(std::ofstream& file)
{
	if (this->_type == TypeType::ArrayT)
	{
		file << dotLabel(this->_id, "Type: " + this->getName());
		file << dotConnection(this->_id, _arrayType->_id);
		this->_arrayType->generateDot(file);
	}
	else
	{
		file << dotLabel(this->_id, "Type: " + this->getName());
	}
}
