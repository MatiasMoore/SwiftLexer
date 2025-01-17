#include "TypeNode.h"
#include "../tables/tables.h"

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

TypeNode* TypeNode::createDynamicType(ExprNode* expr)
{
	auto node = new TypeNode();
	node->_type = DynamicT;
	node->_exprForDynamicT = expr;
	return node;
}

TypeNode* TypeNode::createFromDescriptor(std::string descriptor)
{
	if (descriptor.empty()) {
		throw std::invalid_argument("Descriptor cannot be empty.");
	}

	TypeNode* node = nullptr;
	char firstChar = descriptor[0];

	switch (firstChar) {
	case 'Z':
		node = TypeNode::createType(BoolT);
		break;
	case 'F': 
		node = TypeNode::createType(FloatT);
		break;
	case 'I': 
		node = TypeNode::createType(IntT);
		break;
	case 'C': 
		node = TypeNode::createType(CharacterT);
		break;
	case 'L': { 
		size_t end = descriptor.find(';');
		if (end == std::string::npos) {
			throw std::runtime_error("Invalid descriptor for object type: missing ';'");
		}
		std::string idTypeName = descriptor.substr(1, end - 1); // Extract type name
		node = TypeNode::createIdType(idTypeName);
		break;
	}
	case '[': { 
		TypeNode* arrayType = TypeNode::createFromDescriptor(descriptor.substr(1)); // Recursive call for array element type
		node = TypeNode::createArrayType(arrayType);
		break;
	}
	default:
		throw std::runtime_error("Unsupported descriptor: " + descriptor);
	}

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
		case (TypeType::DynamicT):
			return "Dynamic";
			break;
		default:
		throw std::runtime_error("Can't generate name for type node with enum type " + std::to_string(this->_type));
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

std::string TypeNode::toDescriptor(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	std::string desc = "";
	switch (this->_type)
	{
	case BoolT:
		desc += "Z"; // JVM descriptor for boolean
		break;
	case FloatT:
		desc += "F"; // JVM descriptor for float
		break;
	case IntT:
		desc += "I"; // JVM descriptor for int
		break;
	case StringT:
		desc += "Ljava/lang/String;"; // JVM descriptor for String
		break;
	case CharacterT:
		desc += "C"; // JVM descriptor for char
		break;
	case IdT:
		desc += "L";
		desc += this->_idTypeName;
		desc += ";";
		break;
	case ArrayT:
		desc += "["; // JVM descriptor for array
		desc += this->_arrayType->toDescriptor(classTable, currentClass, currentMethod);
		break;
	default:
		throw std::runtime_error("Unsupported type for descriptor!");
		break;
	}

	return desc;
}
