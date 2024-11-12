#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "ExprNode.h"

FuncCallNode* FuncCallNode::createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs)
{
	auto node = new FuncCallNode();
	node->_hasArgs = true;
	node->_scopeType = FuncCallScopeType::normalCall;
	node->_funcName = funcName;
	node->_funcArgs = funcArgs;
	return node;
}

FuncCallNode* FuncCallNode::createFuncCallNoArgs(std::string funcName)
{
	auto node = new FuncCallNode();
	node->_hasArgs = false;
	node->_scopeType = FuncCallScopeType::normalCall;
	node->_funcName = funcName;
	return node;
}

FuncCallNode* FuncCallNode::setAsExprAccess(ExprNode* exprAccess)
{
	this->_scopeType = exprAccessCall;
	_exprAccess = exprAccess;
	return this;
}

std::string FuncCallNode::getName()
{
	std::string scopeType;
	switch (this->_scopeType)
	{
	case FuncCallScopeType::normalCall:
		scopeType = "Normal call";
		break;
	case FuncCallScopeType::selfCall:
		scopeType = "Self call";
		break;
	case FuncCallScopeType::superCall:
		scopeType = "Super call";
		break;
	case FuncCallScopeType::exprAccessCall:
		scopeType = "Expr access call";
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}

	if (this->_hasArgs)
		return scopeType + " with args";
	else
		return scopeType + " with no args";
}

void FuncCallNode::generateDot(std::ofstream& file)
{
	switch (this->_scopeType)
	{
		case FuncCallScopeType::normalCall:
		case FuncCallScopeType::selfCall:
		case FuncCallScopeType::superCall:
		file << dotLabel(this->_id, this->getName() + ": " + _funcName);
		if (this->_hasArgs)
		{
			file << dotConnection(this->_id, _funcArgs->_id);
			this->_funcArgs->generateDot(file);
		}
			break;
		case FuncCallScopeType::exprAccessCall:
		file << dotLabel(this->_id, this->getName() + ": " + _funcName);
		file << dotConnectionWithLabel(this->_id, _exprAccess->_id, "access");
		this->_exprAccess->generateDot(file);
		if (this->_hasArgs)
		{
			file << dotConnection(this->_id, _funcArgs->_id);
			this->_funcArgs->generateDot(file);
		}
			break;
	}
}
