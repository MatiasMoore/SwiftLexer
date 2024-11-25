#include "ThrowNode.h"
#include "ExprNode.h"

ThrowNode* ThrowNode::createThrowExpr(ExprNode* expr)
{
	auto node = new ThrowNode();
	node->_expr = expr;
	return node;
}

void ThrowNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "Throw");
	file << dotConnection(this->_id, this->_expr->_id);

	this->_expr->generateDot(file);
}
