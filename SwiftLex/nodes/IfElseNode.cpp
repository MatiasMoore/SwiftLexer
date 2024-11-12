#include "IfElseNode.h"
#include "ExprNode.h"
#include "StmtNode.h"

IfElseNode* IfElseNode::createSimple(ExprListNode* conditions, StmtListNode* ifTrue, StmtListNode* elseSimple)
{
	auto node = new IfElseNode();
	node->_type = IfElseNodeType::ifElseSimple;
	node->_conditions = conditions;
	if (ifTrue == nullptr)
	{
		node->_hasIfTrue = false;
	}
	else
	{
		node->_hasIfTrue = true;
		node->_ifTrue = ifTrue;
	}
	if (elseSimple == nullptr)
	{
		node->_hasElse = false;
	}
	else
	{
		node->_hasElse = true;
		node->_elseSimple = elseSimple;
	}
	return node;
}

IfElseNode* IfElseNode::createComplex(ExprListNode* conditions, StmtListNode* ifTrue, IfElseNode* elseComplex)
{
	auto node = new IfElseNode();
	node->_type = IfElseNodeType::ifElseComplex;
	node->_conditions = conditions;
	if (ifTrue == nullptr)
	{
		node->_hasIfTrue = false;
	}
	else
	{
		node->_hasIfTrue = true;
		node->_ifTrue = ifTrue;
	}
	node->_hasElse = true;
	node->_elseComplex = elseComplex;
	return node;
}

void IfElseNode::generateDot(std::ofstream& file)
{
	if (_type == IfElseNodeType::ifElseSimple)
	{
		file << dotLabel(this->_id, "ifElseSimple" + (std::string)(!this->_hasIfTrue ? "\nno true body" : "") + (std::string)(!this->_hasElse ? "\nno else body" : ""));
		file << dotConnectionWithLabel(this->_id, this->_conditions->_id, "condition");
		this->_conditions->generateDot(file);
		if (this->_hasIfTrue)
		{
			file << dotConnectionWithLabel(this->_id, this->_ifTrue->_id, "if true");
			this->_ifTrue->generateDot(file);
		}
		if (this->_hasElse)
		{
			file << dotConnectionWithLabel(this->_id, this->_elseSimple->_id, "else");
			this->_elseSimple->generateDot(file);
		}
	}
	else
	{
		file << dotLabel(this->_id, "ifElseComplex" + (std::string)(!this->_hasIfTrue ? "\nno true body" : "") + (std::string)(!this->_hasElse ? "\nno else body" : ""));
		file << dotConnectionWithLabel(this->_id, this->_conditions->_id, "condition");
		this->_conditions->generateDot(file);
		if (this->_hasIfTrue)
		{
			file << dotConnectionWithLabel(this->_id, this->_ifTrue->_id, "if true");
			this->_ifTrue->generateDot(file);
		}
		if (this->_hasElse)
		{
			file << dotConnectionWithLabel(this->_id, this->_elseComplex->_id, "else");
			this->_elseComplex->generateDot(file);
		}
	}
}
