#include "ExprNode.h"
#include "FuncCallNode.h"
#include "TypeNode.h"

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

std::string ExprListNode::getName()
{
	return "ExprList";
}
