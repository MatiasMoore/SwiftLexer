#include "ExprNode.h"

ExprNode* ExprNode::createBool(bool value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Bool;
	node->_boolValue = value;
	printf("N: bool\n");
	return node;
}

ExprNode* ExprNode::createInt(int value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Int;
	node->_intValue = value;
	printf("N: int\n");
	return node;
}

ExprNode* ExprNode::createFloat(double value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Float;
	node->_floatValue = value;
	printf("N: float\n");
	return node;
}

ExprNode* ExprNode::createString(std::string value)
{
	auto node = new ExprNode();
	node->_type = ExprType::String;
	node->_stringValue = value;
	printf("N: string\n");
	return node;
}

ExprNode* ExprNode::createId(std::string value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Id;
	node->_stringValue = value;
	printf("N: id\n");
	return node;
}

ExprNode* ExprNode::createBinaryOp(ExprType type, ExprNode* left, ExprNode* right)
{
	auto node = new ExprNode();
	node->_type = type;
	node->_left = left;
	node->_right = right;
	printf("N: binaryOp %d\n", (int)type);
	return node;
}

std::string ExprNode::getName()
{
	switch (this->_type)
	{
	case ExprType::Bool:
		return "Bool";
		break;
	case ExprType::Int:
		return "Int";
		break;
	case ExprType::Float:
		return "Float";
		break;
	case ExprType::Mul:
		return "Mul";
		break;
	case ExprType::Div:
		return "Div";
		break;
	case ExprType::Sum:
		return "Sum";
		break;
	case ExprType::Sub:
		return "Sub";
		break;
	case ExprType::String:
		return "String";
		break;
	case ExprType::Id:
		return "Id";
		break;
	case ExprType::Modulus:
		return "Mod %";
		break;
	case ExprType::LT:
		return "LT <";
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

void ExprNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case ExprType::Bool:
		file << dotLabel(this->_id, this->getName() + ": " + (this->_boolValue ? "true" : "false"));
		break;
	case ExprType::Int:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_intValue));
		break;
	case ExprType::Float:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_floatValue));
		break;
	case ExprType::String:
		file << dotLabel(this->_id, this->getName() + ": " + _stringValue);
		break;
	case ExprType::Id:
		file << dotLabel(this->_id, this->getName() + ": " + _stringValue);
		break;
	case ExprType::Sum:
	case ExprType::Sub:
	case ExprType::Div:
	case ExprType::Mul:
	case ExprType::Modulus:
	case ExprType::LT:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_left->_id);
		file << dotConnection(this->_id, this->_right->_id);
		this->_left->generateDot(file);
		this->_right->generateDot(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

