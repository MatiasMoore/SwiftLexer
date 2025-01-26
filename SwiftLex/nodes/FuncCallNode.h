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
	exprAccessCall
};

class FuncCallNode : public Dottable, public SemanticsBase
{
private:
	ExternalMethod* findMethodForCall(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);
	ExternalMethod* findMethodWithTypeCasting(ExternalClass* classWithMethod, std::string methodName, bool shouldBeStatic, ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);
	std::string getArgsDescriptor(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);

public:
	bool _hasArgs;
	FuncCallScopeType _scopeType;

	std::string _funcName;
	FuncCallArgListNode* _funcArgs;

	FuncCallArgListNode* _newFuncArgs;

	ExprNode* _exprAccess;

	int _methodRef = -1;
	bool _isConstructor = false;
	int _classRefForConstructor;
	bool _isStaticMethod = false;
	bool _isSuperCall = false;

	static FuncCallNode* createFuncCall(std::string funcName, FuncCallArgListNode* funcArgs);

	static FuncCallNode* createFuncCallNoArgs(std::string funcName);

	FuncCallNode* setAsExprAccess(ExprNode* exprAccess);

	std::string getName();

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	TypeNode* evaluateType(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

