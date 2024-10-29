#include "ExprNode.h"

ExprNode* createInt(int value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Int;
	node->_intValue = value;
	printf("N: int\n");
	return node;
}

ExprNode* createFloat(double value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Float;
	node->_floatValue = value;
	printf("N: float\n");
	return node;
}

ExprNode* createBinaryOp(ExprType type, ExprNode* left, ExprNode* right)
{
	auto node = new ExprNode();
	node->_type = type;
	node->_left = left;
	node->_right = right;
	printf("N: binaryOp %d\n", (int)type);
	return node;
}

void generateDotExpr(std::ofstream& file, ExprNode* node)
{
	switch (node->_type)
	{
	case ExprType::Int:
		file << dotLabel(node->_id, "Int: " + std::to_string(node->_intValue));
		break;
	case ExprType::Float:
		file << dotLabel(node->_id, "Float: " + std::to_string(node->_floatValue));
		break;
	case ExprType::Sum:
		file << dotLabel(node->_id, "Sum");
		file << dotConnection(node->_id, node->_left->_id);
		file << dotConnection(node->_id, node->_right->_id);
		generateDotExpr(file, node->_left);
		generateDotExpr(file, node->_right);
		break;
	case ExprType::Sub:
		file << dotLabel(node->_id, "Sub");
		file << dotConnection(node->_id, node->_left->_id);
		file << dotConnection(node->_id, node->_right->_id);
		generateDotExpr(file, node->_left);
		generateDotExpr(file, node->_right);
		break;
	case ExprType::Div:
		file << dotLabel(node->_id, "Div");
		file << dotConnection(node->_id, node->_left->_id);
		file << dotConnection(node->_id, node->_right->_id);
		generateDotExpr(file, node->_left);
		generateDotExpr(file, node->_right);
		break;
	case ExprType::Mul:
		file << dotLabel(node->_id, "Mul");
		file << dotConnection(node->_id, node->_left->_id);
		file << dotConnection(node->_id, node->_right->_id);
		generateDotExpr(file, node->_left);
		generateDotExpr(file, node->_right);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

