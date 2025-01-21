#include "IfElseNode.h"
#include "ExprNode.h"
#include "StmtNode.h"
#include "../ExceptionHelper.h"
#include "../generation/generationHelpers.h"
#include "TypeNode.h"

IfElseNode* IfElseNode::createSimple(ExprListNode* conditions, StmtListNode* ifTrue, StmtListNode* else_)
{
	auto node = new IfElseNode();
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
	if (else_ == nullptr)
	{
		node->_hasElse = false;
	}
	else
	{
		node->_hasElse = true;
		node->_else = else_;
	}
	return node;
}

void IfElseNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "ifElse" + (std::string)(!this->_hasIfTrue ? "\nno true body" : "") + (std::string)(!this->_hasElse ? "\nno else body" : ""));
	file << dotConnectionWithLabel(this->_id, this->_conditions->_id, "condition");
	this->_conditions->generateDot(file);
	if (this->_hasIfTrue)
	{
		file << dotConnectionWithLabel(this->_id, this->_ifTrue->_id, "if true");
		this->_ifTrue->generateDot(file);
	}
	if (this->_hasElse)
	{
		file << dotConnectionWithLabel(this->_id, this->_else->_id, "else");
		this->_else->generateDot(file);
	}
}

SemanticsBase* IfElseNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	if (this->_conditions->_vec.size() == 0)
		throw std::runtime_error("If must have an expression inside brackets!" + LINE_AND_FILE);

	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list inside if is not supported!" + LINE_AND_FILE);
	
	this->_conditions = this->_conditions->semanticsTransform(stack)->typecast<ExprListNode>();

	if (this->_hasIfTrue)
	{
		this->_ifTrue = this->_ifTrue->semanticsTransform(stack)->typecast<StmtListNode>();
	}

	if (this->_hasElse)
	{
		this->_else = this->_else->semanticsTransform(stack)->typecast<StmtListNode>();
	}


	return this;
}

void IfElseNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (this->_conditions->_vec.size() == 0)
		throw std::runtime_error("If must have an expression inside brackets!" + LINE_AND_FILE);

	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list inside if is not supported!" + LINE_AND_FILE);

	auto condition = this->_conditions->_vec[0];

	condition->fillTable(classTable, currentClass, currentMethod);

	auto conditionDesc = condition->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

	if (conditionDesc != "Z")
		throw std::runtime_error("Condition inside if else must have a descriptor \"Z\"(bool) but got \"" + conditionDesc + "\"!" + LINE_AND_FILE);

	if (this->_hasIfTrue)
	{
		this->_ifTrue->fillTable(classTable, currentClass, currentMethod);
	}

	if (this->_hasElse)
	{
		this->_else->fillTable(classTable, currentClass, currentMethod);
	}
}

std::vector<char> IfElseNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	
	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list as condition is not supported in if else!" + LINE_AND_FILE);

	auto condition = this->_conditions->_vec[0];

	//Condition must be 1 or 0 
	appendVecToVec(code, condition->generateCode(currentClass, currentMethod));
	appendVecToVec(code, jvm::iconst_1());

	//FIXME Maybe put an empty default instruction there?
	std::vector<char> trueBranch = {};
	std::vector<char> falseBranch = {};

	if (this->_hasIfTrue)
		appendVecToVec(trueBranch, this->_ifTrue->generateCode(currentClass, currentMethod));

	if (this->_hasElse)
		appendVecToVec(falseBranch, this->_else->generateCode(currentClass, currentMethod));

	appendVecToVec(code, jvm::ifElseComplex(IfCommandType::EQ_if, trueBranch, falseBranch));

	return code;
}
