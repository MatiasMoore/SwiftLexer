#pragma once
#include "dottable.h"

class FuncCallArgListNode;
class ExprNode;

enum FuncCallScopeType
{
	normalCall,
	selfCall,
	superCall,
	exprAccessCall
};

class FuncCallNode : public Dottable
{
public:
	bool _hasArgs;
	FuncCallScopeType _scopeType;

	std::string _funcName;
	FuncCallArgListNode* _funcArgs;

	ExprNode* _exprAccess;

	static FuncCallNode* createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs);

	static FuncCallNode* createFuncCallNoArgs(std::string funcName);

	FuncCallNode* setAsExprAccess(ExprNode* exprAccess);

	std::string getName();

	void generateDot(std::ofstream& file) override;
};

