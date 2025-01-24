#include "ExprNode.h"
#include "FuncCallNode.h"
#include "FuncCallArgNode.h"
#include "TypeNode.h"
#include "../generation/generationHelpers.h"
#include <set>
#include "../ExceptionHelper.h"
#include "../tables/tables.h"
#include "../tables/FieldAccessFlag.h"
#include "../RTLHelper.h"

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
	node->_type = ExprType::ArrayCreation;
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
	case ExprType::ArrayCreation:
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
	case ExprType::ArrayCreation:
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
	if (this->_isAlreadyTransformed)
		return this;

	std::set<std::string> fieldAcessToProperty = {
		RTLHelper::_propertyIsEmpty,
	};

	//FIXME should really be done in fillTable because we need to 
	// check type of expr and make sure it's an array
	if (this->_type == ExprType::FieldAccess && this->_fieldAccessFieldName == RTLHelper::_propertyCount)
	{
		//a.count -> Array(a).count()
		auto arrayConst = FuncCallNode::createFuncCall(RTLHelper::_arrayC, FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(this->_fieldAccessExpr)));
		auto countCall = FuncCallNode::createFuncCallNoArgs(RTLHelper::_propertyCount);
		countCall->setAsExprAccess(ExprNode::createFuncCall(arrayConst));
		return ExprNode::createFuncCall(countCall);
	}
	else if (this->_type == ExprType::FieldAccess && fieldAcessToProperty.count(this->_fieldAccessFieldName) != 0)
	{
		//a.property -> a.property()
		auto propertyCall = FuncCallNode::createFuncCallNoArgs(this->_fieldAccessFieldName);
		propertyCall->setAsExprAccess(this->_fieldAccessExpr);
		return ExprNode::createFuncCall(propertyCall);
	}

	std::set<ExprType> typesWithLeftRightOperands = { 
		ExprType::LT, ExprType::GT, ExprType::GTE, ExprType::LTE, ExprType::EQ, ExprType::NEQ, ExprType::Subscript
	};

	std::set<ExprType> doNothingTypes = {
		ExprType::Id, ExprType::FieldAccess, ExprType::SelfFieldAccess
	};

	std::map <ExprType, std::string> binaryExprTypeToTransform = {
		{ ExprType::Sum, RTLHelper::_sum},
		{ ExprType::Sub, RTLHelper::_sub},
		{ ExprType::Mul, RTLHelper::_mul},
		{ ExprType::Div, RTLHelper::_div},
		{ ExprType::LogAnd, RTLHelper::_logAnd},
		{ ExprType::LogOr, RTLHelper::_logOr},
		{ ExprType::LT, RTLHelper::_lt},
		{ ExprType::GT, RTLHelper::_gt},
		{ ExprType::LTE, RTLHelper::_lte}
	};

	std::map <ExprType, std::string> unaryExprTypeToTransform =
	{
		{ ExprType::LogNot, RTLHelper::_logNot },
		{ ExprType::UnaryMinus, RTLHelper::_unaryMinus },
	};

	if (this->_type == ExprType::Int)
	{
		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ExprNode::createInt(this->_intValue)));
		auto newExpr = ExprNode::createFuncCall(FuncCallNode::createFuncCall(RTLHelper::_intC, args));
		newExpr->setIsAlreadyTransformed(true);
		return newExpr;
	}
	else if (this->_type == ExprType::String)
	{
		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ExprNode::createString(this->_stringValue)));
		auto newExpr = ExprNode::createFuncCall(FuncCallNode::createFuncCall(RTLHelper::_strC, args));
		newExpr->setIsAlreadyTransformed(true);
		return newExpr;
	}
	else if (this->_type == ExprType::Bool)
	{
		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ExprNode::createBool(this->_boolValue)));
		auto newExpr = ExprNode::createFuncCall(FuncCallNode::createFuncCall(RTLHelper::_boolC, args));
		newExpr->setIsAlreadyTransformed(true);
		return newExpr;
	}
	else if (this->_type == ExprType::Float)
	{
		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(ExprNode::createFloat(this->_floatValue)));
		auto newExpr = ExprNode::createFuncCall(FuncCallNode::createFuncCall(RTLHelper::_floatC, args));
		newExpr->setIsAlreadyTransformed(true);
		return newExpr;
	}
	else if (binaryExprTypeToTransform.count(this->_type) != 0)
	{
		this->_left = _left->semanticsTransform(stack)->typecast<ExprNode>();
		this->_right = _right->semanticsTransform(stack)->typecast<ExprNode>();

		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(this->_left))->appendNode(FuncCallArgNode::createFromExpr(this->_right));

		auto newFunc = FuncCallNode::createFuncCall(binaryExprTypeToTransform[this->_type], args);
		newFunc->setAsExprAccess(ExprNode::createId(RTLHelper::_internalOpClassName));

		// objA + objB -> Operators.sum(objA, objB)
		// A.sum(objA, objB)
		// Integer.sum(intA, intB)

		return ExprNode::createFuncCall(newFunc);
	}
	else if (unaryExprTypeToTransform.count(this->_type) != 0)
	{
		this->_unary = this->_unary->semanticsTransform(stack)->typecast<ExprNode>();

		auto args = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(this->_unary));

		auto newFunc = FuncCallNode::createFuncCall(unaryExprTypeToTransform[this->_type], args);
		newFunc->setAsExprAccess(ExprNode::createId(RTLHelper::_internalOpClassName));

		return ExprNode::createFuncCall(newFunc);
	}
	else if (this->_type == ExprType::FuncCall)
	{
		this->_funcCall = this->_funcCall->semanticsTransform(stack)->typecast<FuncCallNode>();
	}
	else if (this->_type == ExprType::ArrayCreation)
	{
		this->_arrayExprList = this->_arrayExprList->semanticsTransform(stack)->typecast<ExprListNode>();
	}
	else if (typesWithLeftRightOperands.count(this->_type) != 0)
	{
		this->_left = this->_left->semanticsTransform(stack)->typecast<ExprNode>();
		this->_right = this->_right->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == ExprType::ClosedRange || this->_type == ExprType::HalfOpenRange)
	{
		//a...b -> Range(a, b, isHalfOpen).toArray()
		this->_left = this->_left->semanticsTransform(stack)->typecast<ExprNode>();
		this->_right = this->_right->semanticsTransform(stack)->typecast<ExprNode>();

		auto rangeConstArgs = FuncCallArgListNode::createListNode(
			FuncCallArgNode::createFromExpr(this->_left))
			->appendNode(FuncCallArgNode::createFromExpr(this->_right))
			->appendNode(FuncCallArgNode::createFromExpr(ExprNode::createBool(this->_type == ExprType::HalfOpenRange)));
		auto rangeConst = FuncCallNode::createFuncCall(RTLHelper::_rangeC, rangeConstArgs);
		auto toArrayCall = FuncCallNode::createFuncCallNoArgs(RTLHelper::_toArray);
		toArrayCall->setAsExprAccess(ExprNode::createFuncCall(rangeConst));
		return ExprNode::createFuncCall(toArrayCall);
	}
	else if (doNothingTypes.count(this->_type) != 0)
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
	else if (this->_type == ExprType::Bool)
	{
		return TypeNode::createType(TypeType::BoolT);
	}
	else if (this->_type == ExprType::Float)
	{
		return TypeNode::createType(TypeType::FloatT);
	}
	else if (this->_type == ExprType::Id)
	{
		if (currentMethod == nullptr)
			throw std::runtime_error("This id \"" + this->_stringValue + "\" can't be used in this context!" + LINE_AND_FILE);
		auto localVar = currentMethod->getVarTable()->findLocalVar(this->_stringValue);
		auto nonStaticClassfield = currentClass->findField(this->_stringValue, false);
		bool isLocalVar = localVar != nullptr;
		if (isLocalVar)
		{
			return TypeNode::createFromDescriptor(localVar->_descriptor);
		}
		else if (nonStaticClassfield != nullptr)
		{
			return TypeNode::createFromDescriptor(nonStaticClassfield->getDescriptor());
		}
		else
		{
			throw std::runtime_error("Can't evaluate type for identifier \"" + this->_stringValue + "\"!" + LINE_AND_FILE);
		}
	}
	else if (this->_type == ExprType::FieldAccess)
	{
		std::string classId = "";

		bool isLeftSideComplex = this->_fieldAccessExpr->_type != ExprType::Id;
		bool isLocalVar = !isLeftSideComplex && currentMethod != nullptr && currentMethod->getVarTable()->items.count(this->_fieldAccessExpr->_stringValue);
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
			if (currentMethod == nullptr)
				throw std::runtime_error("Can't use field access with field \"" + this->_fieldAccessFieldName + "\" in this context" + LINE_AND_FILE);

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
	else if (this->_type == ExprType::ArrayCreation)
	{
		return TypeNode::createArrayType(this->_arrayExprList->_vec[0]->evaluateType(classTable, currentClass, currentMethod));
	}
	else if (this->_type == ExprType::Subscript)
	{
		auto arrayType = this->_left->evaluateType(classTable, currentClass, currentMethod);
		if (arrayType->_type != TypeType::ArrayT)
			throw std::runtime_error("Can't evaluate type for subcript because the left expresion in not an array!" + LINE_AND_FILE);

		return arrayType->_arrayType;
	}
	else
	{
		throw std::runtime_error("Can't evaluate type of expr with enum type " + std::to_string(this->_type) + "!");
	}
}

void ExprNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	if (currentClass == nullptr)
		throw std::runtime_error("Expression must be associated with a class!");

	switch (this->_type)
	{
	case ExprType::FuncCall:
		this->_funcCall->fillTable(classTable, currentClass, currentMethod);
		break;

	case ExprType::FieldAccess:
	{
		this->_fieldAccessExpr->fillTable(classTable, currentClass, currentMethod);

		bool isStatic = this->_fieldAccessExpr->_type == ExprType::Id && classTable->findClass(this->_fieldAccessExpr->_stringValue) != nullptr;

		ExternalField* field;

		// Static access
		if (isStatic)
		{
			auto className = this->_fieldAccessExpr->_stringValue;

			auto classElem = classTable->findClass(className);
			if (classElem == nullptr)
				throw std::runtime_error("Unknown identifier \"" + className + "\"in function call!" + LINE_AND_FILE);

			field = classElem->findField(this->_fieldAccessFieldName, true);
			if (field == nullptr)
				throw std::runtime_error("Static field \"" + this->_fieldAccessFieldName + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);
		}
		// Dynamic access
		else
		{
			auto leftDesc = this->_fieldAccessExpr->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
			if (leftDesc[0] != 'L')
				throw std::runtime_error("Primitive types don't have methods!" + LINE_AND_FILE);

			auto className = classnameFromDescriptor(leftDesc);

			auto classElem = classTable->findClass(className);
			if (classElem == nullptr)
				throw std::runtime_error("Critical error! Class \"" + className + "\" is not found!" + LINE_AND_FILE);

			field = classElem->findField(this->_fieldAccessFieldName, false);
			if (field == nullptr)
				throw std::runtime_error("Dynamic field \"" + this->_fieldAccessFieldName + "\" is not defined in class \"" + className + "\"!" + LINE_AND_FILE);
		}

		//Check for permission to access field
		bool isFieldFromOurClass = field->getClassName() == currentClass->getClassName()
			|| classTable->isClassDerivedFromClass(currentClass->getClassName(), field->getClassName());

		bool isFieldPublic = field->containsFlag(FieldAccessFlag::F_ACC_PUBLIC);

		if (!isFieldPublic && !isFieldFromOurClass)
			throw std::runtime_error("Field \"" + field->getVarName() + "\" is not public and is inaccessible from this class \""
				+ currentClass->getClassName() + "\"!" + LINE_AND_FILE);

		_fieldRef = currentClass->getFieldRefForExternalField(field);
		this->_isStaticFieldAccess = field->containsFlag(FieldAccessFlag::F_ACC_STATIC);
	}

		break;
	case ExprType::Int:
		if (this->_intValue < -32767 || this->_intValue > 32767)
		{
			currentClass->getConstTable()->findOrAddInteger(this->_intValue);
		}
		break;
	case ExprType::String:
		currentClass->getConstTable()->findOrAddString(currentClass->getConstTable()->findOrAddUTF8(this->_stringValue));
		break;
	case ExprType::Float:
		currentClass->getConstTable()->findOrAddDouble(this->_floatValue);
		break;
	case ExprType::Bool:
		break;
	case ExprType::Id:
	{
		bool isSelf = this->_stringValue == "self";
		bool isSuper = this->_stringValue == "super";
		if (isSelf)
		{
			auto selfLocalVar = currentMethod->getVarTable()->findLocalVar("self");
			if (selfLocalVar == nullptr)
				throw std::runtime_error("Keyword \"self\" is only allowed inside non-static methods!");

			if (selfLocalVar->_descriptor[0] != 'L')
				throw std::runtime_error("Critical error! \"Self\" must be a link type!" + LINE_AND_FILE);

			auto localVarClassName = classnameFromDescriptor(selfLocalVar->_descriptor);
			if (localVarClassName != currentClass->getClassName())
				throw std::runtime_error("Critical error! \"Self\" must be of type " + currentClass->getClassName() + " but got " + localVarClassName + "!" + LINE_AND_FILE);

			bool isCurrentMethodStatic = currentMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC);
			if (isCurrentMethodStatic)
				throw std::runtime_error("Critical error! Static methods can't have a \"self\" variable defined!" + LINE_AND_FILE);
		}
		else if (isSuper)
		{
			throw std::runtime_error("Super is not supported yet!" + LINE_AND_FILE);
		}
		bool isClassField = currentClass->findField(this->_stringValue, false) != nullptr && currentMethod != nullptr && currentMethod->getVarTable()->findLocalVar(_stringValue) == nullptr;
		if (isClassField)
		{
			auto field = currentClass->findField(this->_stringValue, false);
			_fieldRef = currentClass->getFieldRefForExternalField(field);
		}
	}
		//Do nothing
		break;
	case ExprType::ArrayCreation:
	{
		this->_arrayExprList->fillTable(classTable, currentClass, currentMethod);

		//All elements must be of the same type
		auto firstElemDesc = this->_arrayExprList->_vec[0]->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
		for (int i = 1; i < this->_arrayExprList->_vec.size(); i++)
		{
			auto currentElemDesc = this->_arrayExprList->_vec[i]->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();
			
			//TODO cast ints to floats

			if (firstElemDesc != currentElemDesc)
				throw std::runtime_error("All array elements must be of the same type! Elem at index " + std::to_string(i) + 
					" has descriptor \"" + currentElemDesc + "\" but previous elements have descriptor \"" + firstElemDesc + "\"!" + LINE_AND_FILE);
		}

		this->_arraySize = this->_arrayExprList->_vec.size();
		int nameRef;
		if (firstElemDesc[0] == 'L')
		{
			nameRef = currentClass->getConstTable()->findOrAddUTF8(classnameFromDescriptor(firstElemDesc));
		}
		else if (firstElemDesc[0] == '[')
		{
			nameRef = currentClass->getConstTable()->findOrAddUTF8(firstElemDesc);
		}
		else
		{
			throw std::runtime_error("Only arrays of references are supported!" + LINE_AND_FILE);
		}

		this->_arrayElemDesc = currentClass->getConstTable()->findOrAddClassRef(nameRef);
	}
		break;
	case ExprType::Subscript:
	{
		this->_left->fillTable(classTable, currentClass, currentMethod);

		auto leftDesc = this->_left->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

		if (leftDesc[0] != '[')
			throw std::runtime_error("Subcript can only be used on array expressions but got expression with descriptor \"" + leftDesc + "\"!" + LINE_AND_FILE);

		if (leftDesc[1] != 'L' && leftDesc[1] != '[')
			throw std::runtime_error("Can only subscript from reference arrays!" + LINE_AND_FILE);

		auto rightDesc = this->_right->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

		if (rightDesc != "I")
		{
			if (rightDesc[0] != 'L' || classnameFromDescriptor(rightDesc) != RTLHelper::_intC)
				throw std::runtime_error("Index for subcript must be of type int but got type with descriptor \"" + rightDesc + "\"!" + LINE_AND_FILE);

			auto funcCall = FuncCallNode::createFuncCallNoArgs("toInt");
			funcCall->setAsExprAccess(this->_right);
			auto newRight = ExprNode::createFuncCall(funcCall);
			this->_right = newRight;
		}

		this->_right->fillTable(classTable, currentClass, currentMethod);
	}
	break;
	default:
		throw std::runtime_error("Unsupported expr with enum type " + std::to_string(this->_type) + "!");
		break;
	}
}

std::vector<char> ExprNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	
	LocalVariableElement* localVar;
	ExternalField* nonStaticClassfield;
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
	case ExprType::String:
		appendVecToVec(code, jvm::ldc(currentClass->getConstTable()->findStringRef(currentClass->getConstTable()->findUTF8(this->_stringValue))));
		break;
	case ExprType::Id:
		localVar = currentMethod->getVarTable()->findLocalVar(this->_stringValue);
		nonStaticClassfield = currentClass->findField(this->_stringValue, false);
		if (localVar == nullptr && nonStaticClassfield == nullptr)
			throw std::runtime_error("Critical error! Local var \"" + this->_stringValue + "\" is not defined!" + LINE_AND_FILE);
		if (localVar != nullptr)
		{
			if (localVar->_descriptor == "I")
			{
				appendVecToVec(code, jvm::iload(localVar->localId));
			}
			else if (localVar->_descriptor[0] == 'L')
			{
				appendVecToVec(code, jvm::aload(localVar->localId));
			}
			else if (localVar->_descriptor[0] == '[')
			{
				appendVecToVec(code, jvm::aload(localVar->localId));
			}
			else
			{
				throw std::runtime_error("Can't generate code for expr ID with descriptor \"" + localVar->_descriptor + "\"!");
			}
		}
		else if (nonStaticClassfield != nullptr)
		{
			appendVecToVec(code, jvm::aload(0));
			appendVecToVec(code, jvm::getfield(_fieldRef));
		}

		break;
	case ExprType::FieldAccess:
		if (this->_isStaticFieldAccess) {
			appendVecToVec(code, jvm::getstatic(this->_fieldRef));
		}
		else {
			appendVecToVec(code, this->_fieldAccessExpr->generateCode(currentClass, currentMethod));
			appendVecToVec(code, jvm::getfield(this->_fieldRef));
		}
		
		break;
	case ExprType::ArrayCreation:
	{
		if (this->_arraySize < -32767 || this->_arraySize > 32767)
			throw std::runtime_error("Arrays with size of more than 2 bytes are not supported!" + LINE_AND_FILE);

		appendVecToVec(code, jvm::iconstBipushSipush(this->_arraySize));
		appendVecToVec(code, jvm::anewarray(this->_arrayElemDesc));

		for (int i = 0; i < this->_arrayExprList->_vec.size(); i++)
		{
			appendVecToVec(code, jvm::dup());
			auto currentElem = this->_arrayExprList->_vec[i];
			appendVecToVec(code, jvm::iconstBipushSipush(i));
			appendVecToVec(code, currentElem->generateCode(currentClass, currentMethod));
			appendVecToVec(code, jvm::aastore());
		}
	}
	break;
	case ExprType::Subscript:
	{
		appendVecToVec(code, this->_left->generateCode(currentClass, currentMethod));
		appendVecToVec(code, this->_right->generateCode(currentClass, currentMethod));
		appendVecToVec(code, jvm::aaload());
	}
	break;
	case ExprType::Bool:
		appendVecToVec(code, jvm::iconstBipushSipush(this->_boolValue ? 1 : 0));
		break;
	case ExprType::Float: {

		int floatRef = currentClass->getConstTable()->findDoubleRef(this->_floatValue);
		appendVecToVec(code, jvm::ldc(floatRef));
	}
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

SemanticsBase* ExprListNode::semanticsTransform(SemanticsStack stack)
{
	return SemanticsBase::semanticsTransformVector<ExprNode>(stack, this, _vec);
}

void ExprListNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(classTable, currentClass, currentMethod);
	}
}
