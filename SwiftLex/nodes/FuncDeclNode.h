#pragma once
#include "dottable.h"

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
    OpNILCOALESCE
};

class FuncDeclNode : public Dottable
{
public:
	bool _isGeneric;
	bool _isOperatorOverload;

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

	OverloadableOperatorType _overloadOpearatorType;

	TypeForGenericListNode* _typesForGenericList;

	static FuncDeclNode* createRegular(std::string idName, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createGeneric(std::string idName, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createRegularOperator(OverloadableOperatorType overloadType, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	static FuncDeclNode* createGenericOperator(OverloadableOperatorType overloadType, TypeForGenericListNode* typesForGenericList, FuncDeclArgListNode* argList, StmtListNode* body, TypeNode* returnType, bool throwsException);

	FuncDeclNode* addModifiers(AccessModifierListNode* modifiers);

	std::string getOperatorOverloadName(OverloadableOperatorType overloadType);

	void generateDot(std::ofstream& file) override;
};

