#include "LoopNode.h"
#include "StmtNode.h"
#include "ExprNode.h"

LoopNode* LoopNode::createForLoop(std::string id, ExprNode* iterable, StmtListNode* body)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::forLoop;
	node->_forLoopId = id;
	node->_forLoopIterable = iterable;
	node->_body = body;
	node->_hasBody = true;
	return node;
}

LoopNode* LoopNode::createForLoopNoBody(std::string id, ExprNode* iterable)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::forLoop;
	node->_forLoopId = id;
	node->_forLoopIterable = iterable;
	node->_hasBody = false;
	return node;
}

LoopNode* LoopNode::createRepeatWhileLoop(StmtListNode* body, ExprListNode* condition)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::repeatWhileLoop;
	node->_body = body;
	node->_condition = condition;
	node->_hasBody = true;
	return node;
}

LoopNode* LoopNode::createRepeatWhileLoopNoBody(ExprListNode* condition)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::repeatWhileLoop;
	node->_condition = condition;
	node->_hasBody = false;
	return node;
}

LoopNode* LoopNode::createWhileLoop(ExprListNode* condition, StmtListNode* body)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::whileLoop;
	node->_body = body;
	node->_condition = condition;
	node->_hasBody = true;
	return node;
}

LoopNode* LoopNode::createWhileLoopNoBody(ExprListNode* condition)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::whileLoop;
	node->_condition = condition;
	node->_hasBody = false;
	return node;
}

void LoopNode::generateDot(std::ofstream& file)
{
	std::string bodyStr;
	bodyStr = !this->_hasBody ? "\nno body" : "";
	switch (this->_type)
	{
	case LoopNodeType::forLoop:
		file << dotLabel(this->_id, "ForLoop\n iterable id: " + this->_forLoopId + bodyStr);
		file << dotConnectionWithLabel(this->_id, this->_forLoopIterable->_id, "iterable");
		this->_forLoopIterable->generateDot(file);
		break;
	case LoopNodeType::repeatWhileLoop:
		file << dotLabel(this->_id, "RepeatWhileLoop" + bodyStr);
		file << dotConnectionWithLabel(this->_id, this->_condition->_id, "while");
		this->_condition->generateDot(file);
		break;
	case LoopNodeType::whileLoop:
		file << dotLabel(this->_id, "WhileLoop" + bodyStr);
		file << dotConnectionWithLabel(this->_id, this->_condition->_id, "while");
		this->_condition->generateDot(file);
		break;
	}

	if (this->_hasBody)
	{
		file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
		this->_body->generateDot(file);
	}
}
