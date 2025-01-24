#include "LoopNode.h"
#include "StmtNode.h"
#include "ExprNode.h"
#include "../ExceptionHelper.h"
#include "TypeNode.h"
#include "FuncCallNode.h"
#include "../generation/generationHelpers.h"
#include "VarDeclarationNode.h"
#include "../RTLHelper.h"

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

	if (this->_type == LoopNodeType::whileLoop || this->_type == LoopNodeType::repeatWhileLoop)
	{
		if (this->_conditions->_vec.size() == 0)
			throw std::runtime_error("Loops must have an expression inside brackets!" + LINE_AND_FILE);

		if (this->_conditions->_vec.size() != 1)
			throw std::runtime_error("Expr list inside loops is not supported!" + LINE_AND_FILE);

		this->_conditions = this->_conditions->semanticsTransform(stack)->typecast<ExprListNode>();
	}

	if (this->_type == LoopNodeType::whileLoop)
	{
		if (this->_hasBody)
		{
			this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();
		}
	}
	else if (this->_type == LoopNodeType::repeatWhileLoop)
	{
		if (this->_hasBody)
		{
			this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();
		}
	}
	else if (this->_type == LoopNodeType::forLoop)
	{
		auto myStmt = stack.getClosest<StmtNode>();
		if (myStmt == nullptr)
			throw std::runtime_error("Critical error! Can't find my stmt node!" + LINE_AND_FILE);

		auto myStmtList = stack.getClosest<StmtListNode>();
		if (myStmtList == nullptr)
			throw std::runtime_error("Critical error! Can't find my stmt list node!" + LINE_AND_FILE);

		std::string arrayCopyVarName = RTLHelper::getUniqueInternalVarName();
		std::string indexVarName = RTLHelper::getUniqueInternalVarName();
		std::string countVarName = RTLHelper::getUniqueInternalVarName();

		// var arrayCopy = array
		VarDeclarationListNode* arrayCopyVarDecl = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValue(arrayCopyVarName, this->_forLoopIterable));
		StmtNode* arrayCopyVarDeclStmt = StmtNode::createStmtVarDeclaration(arrayCopyVarDecl);
		myStmtList->appendNodeBeforeNode(arrayCopyVarDeclStmt, myStmt);

		// var index = 0 FIXME should be LET
		VarDeclarationListNode* indexVarDecl = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValue(indexVarName, ExprNode::createInt(0)));
		indexVarDecl->setAsConst(true);
		StmtNode* indexVarDeclStmt = StmtNode::createStmtVarDeclaration(indexVarDecl);
		myStmtList->appendNodeBeforeNode(indexVarDeclStmt, myStmt);

		// var count = arrayCopy.count FIXME should be LET
		ExprNode* getArrCount = ExprNode::createFieldAccessExpr(ExprNode::createId(arrayCopyVarName), "count");
		VarDeclarationListNode* countVarDecl = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValue(countVarName, getArrCount));
		countVarDecl->setAsConst(true);
		StmtNode* countVarDeclStmt = StmtNode::createStmtVarDeclaration(countVarDecl);
		myStmtList->appendNodeBeforeNode(countVarDeclStmt, myStmt);

		// index < count
		ExprListNode* whileCondition = ExprListNode::createListNode(ExprNode::createBinaryOp(ExprType::LT, ExprNode::createId(indexVarName), ExprNode::createId(countVarName)));

		// the newly created stmts and expr above are added before this one, so we have to run semantics on them manually
		arrayCopyVarDeclStmt = arrayCopyVarDeclStmt->semanticsTransform(stack)->typecast<StmtNode>();
		indexVarDeclStmt = indexVarDeclStmt->semanticsTransform(stack)->typecast<StmtNode>();
		countVarDeclStmt = countVarDeclStmt->semanticsTransform(stack)->typecast<StmtNode>();
		whileCondition = whileCondition->semanticsTransform(stack)->typecast<ExprListNode>();

		// var elem = arrayCopy[index] FIXME should be LET
		VarDeclarationListNode* forLoopVarDecl = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValue(this->_forLoopId, 
			ExprNode::createBinaryOp(ExprType::Subscript, ExprNode::createId(arrayCopyVarName), ExprNode::createId(indexVarName))));	
		forLoopVarDecl->setAsConst(true);
		StmtNode* forLoopVarDeclStmt = StmtNode::createStmtVarDeclaration(forLoopVarDecl);

		// index += 1
		StmtNode* incrementIndexVar = StmtNode::createStmtAssignment(ExprNode::createId(indexVarName), 
			ExprNode::createBinaryOp(ExprType::Sum, ExprNode::createId(indexVarName), ExprNode::createInt(1)));
		incrementIndexVar->setSkipConstCheck(true);

		StmtListNode* newBody;
		if (this->_hasBody)
		{
			newBody = this->_body;
			newBody->appendNodeAtIndex(forLoopVarDeclStmt, 0);
			newBody->appendNodeAtIndex(incrementIndexVar, newBody->_vec.size());
		}
		else
		{
			newBody = StmtListNode::createListNode(forLoopVarDeclStmt);
			newBody->appendNode(incrementIndexVar);
		}

		// run semantics for the new body
		newBody = newBody->semanticsTransform(stack)->typecast<StmtListNode>();

		auto newLoop = LoopNode::createWhileLoop(whileCondition, newBody);
		newLoop->setIsAlreadyTransformed(true);

		return newLoop;
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
	else if (this->_type == LoopNodeType::repeatWhileLoop)
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
	else if (this->_type == LoopNodeType::repeatWhileLoop)
	{
		std::vector<char> conditionCode = {};
		appendVecToVec(conditionCode, condition->generateCode(currentClass, currentMethod));

		std::vector<char> loopBodyCode = {};
		if (this->_hasBody)
		{
			appendVecToVec(loopBodyCode, this->_body->generateCode(currentClass, currentMethod));
		}

		appendVecToVec(code, jvm::repeatWhileLoop(conditionCode, loopBodyCode));
	}
	else
	{
		throw std::runtime_error("Loop with enum type " + std::to_string(this->_type) + " is not supported!" + LINE_AND_FILE);
	}
	return code;
}
