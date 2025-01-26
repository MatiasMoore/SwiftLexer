#pragma once
#include <vector>
#include "dottable.h"
#include "GenericListNode.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

class ReturnNode;
class ExprNode;
class LoopNode;
class IfElseNode;
class VarDeclarationListNode;
class FuncDeclNode;
class ThrowNode;
class TryNode;
class DoCatchNode;
class SwitchNode;
class EnumCaseNode;
class EnumDeclarationNode;
class StructDeclarationNode;
class ConstructorDeclNode;
class DestructorDeclNode;
class ClassDeclNode;

enum StmtType
{
	Expr,
	VarDeclarationList,
	Assignment,
	Return,
	Loop,
	IfElse,
	FuncDecl,
	Throw,
	Try,
	Do,
	Switch,
	EnumCase,
	EnumDeclaration,
	StructDeclaration,
	ConstructorDecl,
	DestructorDecl,
	ClassDecl,
	DefaultConstructor
};

class StmtNode : public Dottable, public SemanticsBase
{
public:
	bool _hasSemicolon = false;

	StmtType _type;

	ExprNode* _expr;

	ExprNode* _assignLeft;
	ExprNode* _assignRight;

	ReturnNode* _return;

	ThrowNode* _throw;

	DoCatchNode* _do;

	SwitchNode* _switch;

	EnumCaseNode* _enumCase;
	EnumDeclarationNode* _enumDeclaration;

	StructDeclarationNode* _structDeclaration;

	TryNode* _tryNode;

	LoopNode* _loop;

	IfElseNode* _ifElse;

	VarDeclarationListNode* _varDeclList;

	FuncDeclNode* _funcDecl;

	ConstructorDeclNode* _constructorDecl;

	DestructorDeclNode* _destructorDecl;

	ClassDeclNode* _classDecl;

	std::string _assignDesc;
	bool _assignArrayElem = false;
	ExprNode* _assignArray;
	ExprNode* _assignArrayIndex;

	std::string _defaultConstructorBaseName;

	bool _isDynamicClassFieldAssignment = false;
	int _dynamicClassFieldAssignmentRef;

	int _defaultConstructorClassRef;
	int _defaultConstructorMethodRef;

	bool _assignmentSkipConstCheck = false;

	void setSkipConstCheck(bool flag);

	static StmtNode* createStmtExpr(ExprNode* expr);

	static StmtNode* createStmtAssignment(ExprNode* left, ExprNode* right);

	static StmtNode* createStmtVarDeclaration(VarDeclarationListNode* varDeclList);

	static StmtNode* createStmtReturn(ReturnNode* ret);

	static StmtNode* createStmtLoop(LoopNode* loop);

	static StmtNode* createStmtIfElse(IfElseNode* ifElse);

	static StmtNode* createStmtFuncDecl(FuncDeclNode* funcDecl);

	static StmtNode* createStmtThrow(ThrowNode* throwNode);

	static StmtNode* createStmtTry(TryNode* tryNode);

	static StmtNode* createStmtDo(DoCatchNode* doCatch);

	static StmtNode* createStmtSwitch(SwitchNode* switchNode);

	static StmtNode* createStmtEnumCase(EnumCaseNode* enumCase);

	static StmtNode* createStmtEnumDeclaration(EnumDeclarationNode* enumCase);

	static StmtNode* createStmtStructDeclaration(StructDeclarationNode* structDeclaration);

	static StmtNode* createStmtConstructorDecl(ConstructorDeclNode* constructor);

	static StmtNode* createStmtDestructorDecl(DestructorDeclNode* destructor);

	static StmtNode* createClassDecl(ClassDeclNode* classDecl);

	static StmtNode* createDefaultClassConstructor(std::string baseClassName);

	void generateDot(std::ofstream& file) override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan = false);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};

class StmtListNode : public GenericListNode<StmtNode, StmtListNode>, public SemanticsBase
{
public:
	bool _isLogAndBufInitialized = false;
	std::string getName() override;

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan = false);

	std::vector<char> generateCode(InternalClass* currentClass, InternalMethod* currentMethod);
};
