#include "LoopNode.h"
#include "StmtNode.h"
#include "ExprNode.h"
#include "../ExceptionHelper.h"
#include "TypeNode.h"
#include "FuncCallNode.h"
#include "../generation/generationHelpers.h"

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
	node->_conditions = condition;
	node->_hasBody = true;
	return node;
}

LoopNode* LoopNode::createRepeatWhileLoopNoBody(ExprListNode* condition)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::repeatWhileLoop;
	node->_conditions = condition;
	node->_hasBody = false;
	return node;
}

LoopNode* LoopNode::createWhileLoop(ExprListNode* condition, StmtListNode* body)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::whileLoop;
	node->_body = body;
	node->_conditions = condition;
	node->_hasBody = true;
	return node;
}

LoopNode* LoopNode::createWhileLoopNoBody(ExprListNode* condition)
{
	auto node = new LoopNode();
	node->_type = LoopNodeType::whileLoop;
	node->_conditions = condition;
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
		file << dotConnectionWithLabel(this->_id, this->_conditions->_id, "while");
		this->_conditions->generateDot(file);
		break;
	case LoopNodeType::whileLoop:
		file << dotLabel(this->_id, "WhileLoop" + bodyStr);
		file << dotConnectionWithLabel(this->_id, this->_conditions->_id, "while");
		this->_conditions->generateDot(file);
		break;
	}

	if (this->_hasBody)
	{
		file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
		this->_body->generateDot(file);
	}
}

SemanticsBase* LoopNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	if (this->_conditions->_vec.size() == 0)
		throw std::runtime_error("Loops must have an expression inside brackets!" + LINE_AND_FILE);

	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list inside loops is not supported!" + LINE_AND_FILE);

	this->_conditions = this->_conditions->semanticsTransform(stack)->typecast<ExprListNode>();

	if (this->_type == LoopNodeType::whileLoop)
	{
		if (this->_hasBody)
		{
			this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();
		}
	}
	else
	{
		throw std::runtime_error("Loop with enum type " + std::to_string(this->_type) + " is not supported!" + LINE_AND_FILE);
	}

	return this;
}

void LoopNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (this->_conditions->_vec.size() == 0)
		throw std::runtime_error("Loops must have an expression inside brackets!" + LINE_AND_FILE);

	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list inside loops not supported!" + LINE_AND_FILE);

	auto condition = this->_conditions->_vec[0];

	auto conditionDesc = condition->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

	//Cast to bool 
	if (conditionDesc != "Z")
	{
		auto funcCall = FuncCallNode::createFuncCallNoArgs("toBool");
		funcCall->setAsExprAccess(condition);
		auto newCondition = ExprNode::createFuncCall(funcCall);
		this->_conditions->_vec[0] = newCondition;
		condition = newCondition;
	}

	condition->fillTable(classTable, currentClass, currentMethod);


	if (this->_type == LoopNodeType::whileLoop)
	{
		if (this->_hasBody)
		{
			this->_body->fillTable(classTable, currentClass, currentMethod);
		}
	}
	else
	{
		throw std::runtime_error("Loop with enum type " + std::to_string(this->_type) + " is not supported!" + LINE_AND_FILE);
	}
}

std::vector<char> LoopNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};

	if (this->_conditions->_vec.size() != 1)
		throw std::runtime_error("Expr list as condition is not supported in if else!" + LINE_AND_FILE);

	auto condition = this->_conditions->_vec[0];

	if (this->_type == LoopNodeType::whileLoop)
	{
		std::vector<char> conditionCode = {};
		appendVecToVec(conditionCode, condition->generateCode(currentClass, currentMethod));

		std::vector<char> loopBodyCode = {};
		if (this->_hasBody)
		{
			appendVecToVec(loopBodyCode, this->_body->generateCode(currentClass, currentMethod));
		}

		appendVecToVec(code, jvm::whileLoop(conditionCode, loopBodyCode));
	}
	else
	{
		throw std::runtime_error("Loop with enum type " + std::to_string(this->_type) + " is not supported!" + LINE_AND_FILE);
	}
	return code;
}
