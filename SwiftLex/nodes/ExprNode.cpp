#include "ExprNode.h"

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
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

void ExprNode::generateDotExpr(std::ofstream& file)
{
	switch (this->_type)
	{
	case ExprType::Int:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_intValue));
		break;
	case ExprType::Float:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_floatValue));
		break;
	case ExprType::Sum:
	case ExprType::Sub:
	case ExprType::Div:
	case ExprType::Mul:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_left->_id);
		file << dotConnection(this->_id, this->_right->_id);
		this->_left->generateDotExpr(file);
		this->_right->generateDotExpr(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

