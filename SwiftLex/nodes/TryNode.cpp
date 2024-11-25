#include "TryNode.h"
#include "ExprNode.h"

TryNode* TryNode::create(ExprNode* expr)
{
	auto node = new TryNode();

	node->_expr = expr;

	return node;
}

void TryNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "Try");
	file << dotConnection(this->_id, this->_expr->_id);

	this->_expr->generateDot(file);
}


