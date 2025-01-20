#include "ExprNode.h"
#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "TypeNode.h"
#include "../generation/generationHelpers.h"
#include <set>
#include "../ExceptionHelper.h"
#include "../tables/tables.h"
#include "../tables/FieldAccessFlag.h"

ExprNode* ExprNode::createBool(bool value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Bool;
	node->_boolValue = value;
	printf("N: bool\n");
	return node;
}

ExprNode* ExprNode::createInt(int value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Int;
	node->_intValue = value;
	printf("N: int\n");
	return node;
}

ExprNode* ExprNode::createFloat(double value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Float;
	node->_floatValue = value;
	printf("N: float\n");
	return node;
}

ExprNode* ExprNode::createString(std::string value)
{
	auto node = new ExprNode();
	node->_type = ExprType::String;
	node->_stringValue = value;
	printf("N: string\n");
	return node;
}

ExprNode* ExprNode::createId(std::string value)
{
	auto node = new ExprNode();
	node->_type = ExprType::Id;
	node->_stringValue = value;
	printf("N: id\n");
	return node;
}

ExprNode* ExprNode::createBinaryOp(ExprType type, ExprNode* left, ExprNode* right)
{
	auto node = new ExprNode();
	node->_type = type;
	node->_left = left;
	node->_right = right;
	printf("N: binaryOp %d\n", (int)type);
	return node;
}

ExprNode* ExprNode::createUnaryOp(ExprType type, ExprNode* unary)
{
	auto node = new ExprNode();
	node->_type = type;
	node->_unary = unary;
	printf("N: unaryOp %d\n", (int)type);
	return node;
}

ExprNode* ExprNode::createTernary(ExprNode* condition, ExprNode* ifTrue, ExprNode* ifFalse)
{
	auto node = new ExprNode();
	node->_type = ExprType::Ternary;
	node->_ternaryCondition = condition;
	node->_ternaryIfTrue = ifTrue;
	node->_ternaryIfFalse = ifFalse;
	printf("N: ternaryOp %d\n", (int)node->_type);
	return node;
}

ExprNode* ExprNode::createArray(ExprListNode* list)
{
	auto node = new ExprNode();
	node->_type = ExprType::Array;
	node->_arrayExprList = list;
	printf("N: array %d\n", (int)node->_type);
	return node;
}

ExprNode* ExprNode::createFuncCall(FuncCallNode* func)
{
	auto node = new ExprNode();
	node->_type = ExprType::FuncCall;
	node->_funcCall = func;
	printf("N: func call %d\n", (int)node->_type);
	return node;
}

ExprNode* ExprNode::createFieldAccessExpr(ExprNode* expr, std::string fieldName)
{
	auto node = new ExprNode();
	node->_type = ExprType::FieldAccess;
	node->_fieldAccessExpr = expr;
	node->_fieldAccessFieldName = fieldName;
	printf("N: field access %d\n", (int)node->_type);
	return node;
}

ExprNode* ExprNode::createFieldAccessSelf(std::string fieldName)
{
	auto node = new ExprNode();
	node->_type = ExprType::SelfFieldAccess;
	node->_fieldAccessFieldName = fieldName;
	printf("N: field access %d\n", (int)node->_type);
	return node;
}

ExprNode* ExprNode::createTypeCheck(ExprNode* toCheck, TypeNode* type)
{
	auto node = new ExprNode();
	node->_type = ExprType::TypeCheck;
	node->_typeCheckCastExpr = toCheck;
	node->_typeCheckCastType = type;
	return node;
}

ExprNode* ExprNode::createTypeCast(ExprNode* toCheck, TypeNode* type)
{
	auto node = new ExprNode();
	node->_type = ExprType::TypeCast;
	node->_typeCheckCastExpr = toCheck;
	node->_typeCheckCastType = type;
	return node;
}

ExprNode* ExprNode::createTypeCastWithThrow(ExprNode* toCheck, TypeNode* type)
{
	auto node = new ExprNode();
	node->_type = ExprType::TypeCastWithThrow;
	node->_typeCheckCastExpr = toCheck;
	node->_typeCheckCastType = type;
	return node;
}

ExprNode* ExprNode::createTypeCastWithCheck(ExprNode* toCheck, TypeNode* type)
{
	auto node = new ExprNode();
	node->_type = ExprType::TypeCastWithCheck;
	node->_typeCheckCastExpr = toCheck;
	node->_typeCheckCastType = type;
	return node;
}

std::string ExprNode::getName()
{
	switch (this->_type)
	{
	case ExprType::Bool:
		return "Bool";
		break;
	case ExprType::Int:
		return "Int";
		break;
	case ExprType::Float:
		return "Float";
		break;
	case ExprType::Mul:
		return "Mul";
		break;
	case ExprType::Div:
		return "Div";
		break;
	case ExprType::Sum:
		return "Sum";
		break;
	case ExprType::Sub:
		return "Sub";
		break;
	case ExprType::String:
		return "String";
		break;
	case ExprType::Id:
		return "Id";
		break;
	case ExprType::Modulus:
		return "Mod %";
		break;
	case ExprType::LT:
		return "LT <";
		break;
	case ExprType::GT:
		return "GT >";
		break;
	case ExprType::GTE:
		return "GTE >=";
		break;
	case ExprType::LTE:
		return "LTE <=";
		break;
	case ExprType::EQ:
		return "EQ ==";
		break;
	case ExprType::NEQ:
		return "NEQ !=";
		break;
	case ExprType::BitAnd:
		return "BitAnd &";
		break;
	case ExprType::BitOr:
		return "BitOr |";
		break;
	case ExprType::BitXor:
		return "BitXor ^";
		break;
	case ExprType::LogAnd:
		return "LogAnd &&";
		break;
	case ExprType::LogOr:
		return "LogOr ||";
		break;
	case ExprType::Lshift:
		return "Lshift <<";
		break;
	case ExprType::Rshift:
		return "Rshift >>";
		break;
	case ExprType::ClosedRange:
		return "ClosedRange ...";
		break;
	case ExprType::HalfOpenRange:
		return "HalfOpenRange ..<";
		break;
	case ExprType::NilCoalesce:
		return "NilCoalesce ??";
		break;
	case ExprType::BitNot:
		return "BitNot ~";
		break;
	case ExprType::LogNot:
		return "LogNot !";
		break;
	case ExprType::UnaryMinus:
		return "UnaryMinus -";
		break;
	case ExprType::Ternary:
		return "Ternary ? :";
		break;
	case ExprType::Array:
		return "Array";
		break;
	case ExprType::Subscript:
		return "Subscript []";
		break;
	case ExprType::FuncCall:
		return "FuncCall";
		break;
	case ExprType::FieldAccess:
		return "FieldAccess";
		break;
	case ExprType::SelfFieldAccess:
		return "SelfFieldAccess";
		break;
	case ExprType::TypeCheck:
		return "TypeCheck is";
		break;
	case ExprType::TypeCast:
		return "TypeCast as";
		break;
	case ExprType::TypeCastWithThrow:
		return "TypeCastWithThrow as!";
		break;
	case ExprType::TypeCastWithCheck:
		return "TypeCastWithCheck as?";
		break;
	case ExprType::BinaryNot:
		return "BinaryNot";
		break;
	case ExprType::PrefixPlus:
		return "PrefixPlus";
		break;
	case ExprType::PostfixPlus:
		return "PostfixPlus";
		break;
	case ExprType::PrefixDiv:
		return "PrefixDiv";
		break;
	case ExprType::PostfixDiv:
		return "PostfixDiv";
		break;
	case ExprType::PrefixMul:
		return "PrefixMul";
		break;
	case ExprType::PostfixMul:
		return "PostfixMul";
		break;
	case ExprType::PostfixMod:
		return "PostfixMod";
		break;
	case ExprType::PrefixMod:
		return "PrefixMod";
		break;
	case ExprType::PrefixLogAnd:
		return "PrefixLogAnd";
		break;
	case ExprType::PostfixLogAnd:
		return "PostfixLogAnd";
		break;
	case ExprType::PrefixLogOr:
		return "PrefixLogOr";
		break;
	case ExprType::PostfixLogOr:
		return "PostfixLogOr";
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

void ExprNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case ExprType::Bool:
		file << dotLabel(this->_id, this->getName() + ": " + (this->_boolValue ? "true" : "false"));
		break;
	case ExprType::Int:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_intValue));
		break;
	case ExprType::Float:
		file << dotLabel(this->_id, this->getName() + ": " + std::to_string(this->_floatValue));
		break;
	case ExprType::String:
		file << dotLabel(this->_id, this->getName() + ": " + _stringValue);
		break;
	case ExprType::Id:
		file << dotLabel(this->_id, this->getName() + ": " + _stringValue);
		break;
	case ExprType::Ternary:
		file << dotLabel(this->_id, this->getName());
		file << dotConnectionWithLabel(this->_id, this->_ternaryCondition->_id, "condition");
		file << dotConnectionWithLabel(this->_id, this->_ternaryIfTrue->_id, "ifTrue");
		file << dotConnectionWithLabel(this->_id, this->_ternaryIfFalse->_id, "ifFalse");
		this->_ternaryCondition->generateDot(file);
		this->_ternaryIfTrue->generateDot(file);
		this->_ternaryIfFalse->generateDot(file);
		break;
	case ExprType::Array:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_arrayExprList->_id);
		this->_arrayExprList->generateDot(file);
		break;
	case ExprType::BitNot:
	case ExprType::LogNot:
	case ExprType::UnaryMinus:
	case ExprType::PrefixPlus:
	case ExprType::PostfixPlus:
	case ExprType::PrefixDiv:
	case ExprType::PostfixDiv:
	case ExprType::PrefixMul:
	case ExprType::PostfixMul:
	case ExprType::PostfixMod:
	case ExprType::PrefixMod:
	case ExprType::PrefixLogAnd:
	case ExprType::PostfixLogAnd:
	case ExprType::PrefixLogOr:
	case ExprType::PostfixLogOr:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_unary->_id);
		this->_unary->generateDot(file);
		break;
	case ExprType::FieldAccess:
		file << dotLabel(this->_id, this->getName() + "\n fieldId: " + this->_fieldAccessFieldName);
		file << dotConnection(this->_id, this->_fieldAccessExpr->_id);
		this->_fieldAccessExpr->generateDot(file);
		break;
	case ExprType::SelfFieldAccess:
		file << dotLabel(this->_id, this->getName() + "\n fieldId: " + this->_fieldAccessFieldName);
		break;
	case ExprType::TypeCheck:
	case ExprType::TypeCast:
	case ExprType::TypeCastWithCheck:
	case ExprType::TypeCastWithThrow:
		file << dotLabel(this->_id, this->getName());
		file << dotConnectionWithLabel(this->_id, this->_typeCheckCastExpr->_id, "to cast/check");
		file << dotConnectionWithLabel(this->_id, this->_typeCheckCastType->_id, "type");
		this->_typeCheckCastExpr->generateDot(file);
		this->_typeCheckCastType->generateDot(file);
		break;
	case ExprType::Sum:
	case ExprType::Sub:
	case ExprType::Div:
	case ExprType::Mul:
	case ExprType::Modulus:
	case ExprType::LT:
	case ExprType::GT:
	case ExprType::GTE:
	case ExprType::LTE:
	case ExprType::EQ:
	case ExprType::NEQ:
	case ExprType::BitAnd:
	case ExprType::BitOr:
	case ExprType::BitXor:
	case ExprType::LogAnd:
	case ExprType::LogOr:
	case ExprType::Lshift:
	case ExprType::Rshift:
	case ExprType::ClosedRange:
	case ExprType::HalfOpenRange:
	case ExprType::NilCoalesce:
	case ExprType::BinaryNot:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_left->_id);
		file << dotConnection(this->_id, this->_right->_id);
		this->_left->generateDot(file);
		this->_right->generateDot(file);
		break;
	case ExprType::Subscript:
		file << dotLabel(this->_id, this->getName());
		file << dotConnectionWithLabel(this->_id, this->_left->_id, "array");
		file << dotConnectionWithLabel(this->_id, this->_right->_id, "index");
		this->_left->generateDot(file);
		this->_right->generateDot(file);
		break;
	case ExprType::FuncCall:
		file << dotLabel(this->_id, this->getName());
		file << dotConnection(this->_id, this->_funcCall->_id);
		this->_funcCall->generateDot(file);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

SemanticsBase* ExprNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);

	std::set<ExprType> typesWithLeftRightOperands = { 
		ExprType::LT, ExprType::GT, ExprType::GTE, ExprType::LTE, ExprType::EQ, ExprType::NEQ, 
	};

	if (this->_type == ExprType::Int)
	{
		//auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ExprNode::createInt(this->_intValue)));
		//return ExprNode::createFuncCall(FuncCallNode::createFuncCall("Integer", args));
		return this;
	}
	else if (this->_type == ExprType::Sum)
	{
		this->_left = _left->semanticsTransform(stack)->typecast<ExprNode>();
		this->_right = _right->semanticsTransform(stack)->typecast<ExprNode>();

		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(this->_right));

		auto plusFunc = FuncCallNode::createFuncCall("plus", args);
		plusFunc->setAsExprAccess(this->_left);

		return ExprNode::createFuncCall(plusFunc);
	}
	else if (this->_type == ExprType::FuncCall)
	{
		this->_funcCall = this->_funcCall->semanticsTransform(stack)->typecast<FuncCallNode>();
	}
	else if (typesWithLeftRightOperands.count(this->_type) != 0)
	{
		this->_left = this->_left->semanticsTransform(stack)->typecast<ExprNode>();
		this->_right = this->_right->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == ExprType::Id || this->_type == ExprType::FieldAccess)
	{
		//Do nothing
	}
	else
	{
		throw std::runtime_error("Unsupported expr with enum type " + std::to_string(this->_type) + "!");
	}
	return this;
}

TypeNode* ExprNode::evaluateType(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (this->_type == ExprType::String)
	{
		return TypeNode::createType(TypeType::StringT);
	}
	else if (this->_type == ExprType::Int)
	{
		return TypeNode::createType(TypeType::IntT);
	}
	else if (this->_type == ExprType::Id)
	{
		auto localVar = currentMethod->getVarTable()->findLocalVar(this->_stringValue);
		bool isLocalVar = localVar != nullptr;
		if (isLocalVar)
		{
			return TypeNode::createFromDescriptor(localVar->_descriptor);
		}
		else
		{
			auto classElem = classTable->findClass(this->_stringValue);
			bool isClass = classElem != nullptr;
			if (!isClass)
				throw std::runtime_error("Unknown identifier \"" + this->_stringValue + "\"!" + LINE_AND_FILE);

			return TypeNode::createIdType(this->_stringValue);
		}
	}
	else if (this->_type == ExprType::FieldAccess)
	{
		std::string classId = "";

		bool isLeftSideComplex = this->_fieldAccessExpr->_type != ExprType::Id;
		bool isLocalVar = !isLeftSideComplex && currentMethod->getVarTable()->items.count(this->_fieldAccessExpr->_stringValue);
		bool isStatic = !isLeftSideComplex && !isLocalVar;

		if (isLeftSideComplex)
		{
			auto leftSideType = this->_fieldAccessExpr->evaluateType(classTable, currentClass, currentMethod);
			if (leftSideType->_type != TypeType::IdT)
				throw std::runtime_error("Left side of a complex field access expects id, but got type with enum " + std::to_string(leftSideType->_type) + "!");

			classId = leftSideType->_idTypeName;
		}

		// Static field
		if (isStatic)
		{

			bool isStaticFieldFound = false;
			if (this->_fieldAccessExpr->_type == ExprType::Id) {
				auto field = classTable->findClass(this->_fieldAccessExpr->_stringValue)->findField(this->_fieldAccessFieldName, true);
				if (field != nullptr) {
					isStaticFieldFound = field->containsFlag(FieldAccessFlag::F_ACC_STATIC);
				}
			}

			if (!isStaticFieldFound)
				throw std::runtime_error("Critical error! Field access is neither static or non-static!");

			classId = this->_fieldAccessExpr->_stringValue;
		}
		else if (classId.empty())// Non-static field with unknown class id
		{
			auto localVar = currentMethod->getVarTable()->findLocalVar(this->_fieldAccessExpr->_stringValue);
			if (localVar->_descriptor.size() == 1)
				throw std::runtime_error("Primitive types can't be used with field access!");

			classId = classnameFromDescriptor(localVar->_descriptor);
		}

		bool classFound = classTable->findClass(classId) != nullptr;

		if (!classFound)
			throw std::runtime_error("Class \"" + classId + "\" is not found for field access!");

		bool fieldFound = classTable->findClass(classId)->findField(this->_fieldAccessFieldName, isStatic) != nullptr; 	
		if (!fieldFound)
			throw std::runtime_error("Field \"" + this->_fieldAccessFieldName + "\" is not found in class \"" + classId + "\"!");

		auto field = classTable->findClass(classId)->findField(this->_fieldAccessFieldName, isStatic);

		return TypeNode::createFromDescriptor(field->getDescriptor());
	}
	else if (this->_type == ExprType::FuncCall)
	{
		return this->_funcCall->evaluateType(classTable, currentClass, currentMethod);
	}
	else
	{
		throw std::runtime_error("Can't evaluate type of expr with enum type " + std::to_string(this->_type) + "!");
	}
}

void ExprNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	ExternalField* field;
	if (currentClass == nullptr)
		throw std::runtime_error("Expression must be associated with a class!");
/*  It is class Field
	if (currentMethod == nullptr)
		throw std::runtime_error("Expression must be inside a method!");
*/
	switch (this->_type)
	{
	case ExprType::FuncCall:
		this->_funcCall->fillTable(classTable, currentClass, currentMethod);
		break;

	case ExprType::FieldAccess:
		//TODO non static field access; for reference see ExprNode::evaluateType
		if (this->_fieldAccessExpr->_type != Id)
		{
			throw std::runtime_error("Unsupported field access with left part" + std::to_string(_fieldAccessExpr->_type) + "!" + "Field access only support \"ID\" at left part! ");
		}

		this->_staticFieldRef = currentClass->getFieldRefForExternalField(
			classTable->findField(
				this->_fieldAccessFieldName,
				_fieldAccessExpr->_stringValue,
				true
			)
		);
		this->_isStaticFieldAccess = true;
		break;
	case ExprType::Int:
		if (this->_intValue < -32767 || this->_intValue > 32767)
		{
			currentClass->getConstTable()->findOrAddInteger(this->_intValue);
		}
		break;
	case ExprType::Id:
		break;
	default:
		throw std::runtime_error("Unsupported expr with enum type " + std::to_string(this->_type) + "!");
		break;
	}
}

std::vector<char> ExprNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	
	LocalVariableElement* localVar;
	std::vector<char> code = {};
	// TODO change switch to if else
	switch (this->_type)
	{
	case ExprType::FuncCall:
		appendVecToVec(code, this->_funcCall->generateCode(currentClass, currentMethod));
		break;
	case ExprType::Int:
		if (this->_intValue < -32767 || this->_intValue > 32767) {
			appendVecToVec(code, jvm::ldc(currentClass->getConstTable()->findIntegerRef(this->_intValue)));
		}
		else {
			appendVecToVec(code, jvm::iconstBipushSipush(this->_intValue));
		}
		break;
	case ExprType::Id:

		localVar = currentMethod->getVarTable()->findLocalVar(this->_stringValue);
		if (localVar->_descriptor == "I")
		{
			appendVecToVec(code, jvm::iload(localVar->localId));
		}
		else
		{
			throw std::runtime_error("Can't generate code for expr ID with descriptor \"" + localVar->_descriptor + "\"!");
		}
		break;
	case ExprType::FieldAccess:
		appendVecToVec(code, jvm::getstatic(this->_staticFieldRef));
		break;
	default:
		throw std::runtime_error("Can't generate code for expr with type " + std::to_string(this->_type) + "!");
		break;
	}
	return code;
}

std::string ExprListNode::getName()
{
	return "ExprList";
}
