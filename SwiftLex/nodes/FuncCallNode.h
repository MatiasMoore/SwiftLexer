#pragma once
#include "dottable.h"
#include <vector>
#include "SemanticsBase.h"
#include "../tables/tables.h"
#include <set>

class FuncCallArgListNode;
class ExprNode;
class TypeNode;

enum FuncCallScopeType
{
	normalCall,
	selfCall,
	superCall,
	exprAccessCall
};

class FuncCallNode : public Dottable, public SemanticsBase
{
private:
	ExternalMethod* findMethodForCall(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

public:
	bool _hasArgs;
	FuncCallScopeType _scopeType;

	std::string _funcName;
	FuncCallArgListNode* _funcArgs;

	ExprNode* _exprAccess;

	int _methodRef = -1;
	std::vector<MethodAccessFlag> _methodFlags = {};
	bool _isConstructor = false;
	int _classRefForConstructor;

	static FuncCallNode* createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs);

	static FuncCallNode* createFuncCallNoArgs(std::string funcName);

	FuncCallNode* setAsExprAccess(ExprNode* exprAccess);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	TypeNode* evaluateType(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

