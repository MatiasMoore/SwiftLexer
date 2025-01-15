#pragma once
#include "dottable.h"
#include <vector>

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

	int _methodRef = -1;

	static FuncCallNode* createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs);

	static FuncCallNode* createFuncCallNoArgs(std::string funcName);

	FuncCallNode* setAsExprAccess(ExprNode* exprAccess);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	void fillTable(class ClassTable* classTable, class ClassTableElement* currentClass, class MethodTableElement* currentMethod);

	std::vector<char> generateCode(class ClassTableElement* currentClass, class MethodTableElement* currentMethod);
};

