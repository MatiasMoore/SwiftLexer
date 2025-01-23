#pragma once
#include "dottable.h"
#include "SemanticsBase.h"
#include "../tables/tables.h"

class StmtListNode;
class FuncDeclArgListNode;
class TypeForGenericListNode;
class TypeNode;
class AccessModifierListNode;

enum OverloadableOperatorType
{
    OpPLUS,
    OpMINUS,
    OpMUL,
    OpDIV,
    OpMOD,
	OpLT,
	OpGT,
    OpGTE,
    OpLTE,
    OpEQ,
    OpNEQ,
    OpBitAND,
    OpBitOR,
    OpBitXOR,
    OpLogAND,
    OpLogOR,
    OpLSHIFT,
    OpRSHIFT,
    OpCLOSEDRANGE,
    OpHALFOPENRANGE,
    OpNILCOALESCE,
	opPrefixMinus
};

enum OverloadOperatorNotation
{
	Postfix,
	Prefix,
	Infix
};

class FuncDeclNode : public Dottable, public SemanticsBase
{
public:
	bool _isOperatorOverload;
	OverloadOperatorNotation _overloadNotation;

	bool _hasModifiers;
	bool _hasArgs;
	bool _hasBody;
	bool _hasNonVoidReturn;
	bool _throwsException;

	AccessModifierListNode* _modifiers;
	std::string _idName;
	FuncDeclArgListNode* _argList;
	StmtListNode* _body;
	TypeNode* _returnType;

	OverloadableOperatorType _overloadOperatorType;

	InternalMethod* _scannedMethod;

	static FuncDeclNode* createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createRegularOperator(OverloadOperatorNotation overloadNotation, OverloadableOperatorType overloadOperatorType, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	FuncDeclNode* addModifiers(AccessModifierListNode* modifiers);

	std::string getOperatorOverloadName(OverloadableOperatorType overloadOperatorType, OverloadOperatorNotation overloadOperatorNotation);

	void generateDot(std::ofstream& file) override;

	void fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, bool initialScan = false);

	SemanticsBase* semanticsTransform(SemanticsStack stack) override;
};

