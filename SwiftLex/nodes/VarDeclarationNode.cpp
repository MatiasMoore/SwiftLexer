#include "VarDeclarationNode.h"
#include "TypeNode.h"
#include "ExprNode.h"
#include "AccessModifierNode.h"
#include "StmtNode.h"
#include "../tables/tables.h"
#include "../generation/generationHelpers.h"

VarDeclarationNode* VarDeclarationNode::createFromValue(std::string varName, ExprNode* value)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueKnown;
	node->_valueNode = value;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromType(std::string varName, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = TypeKnown;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueAndTypeKnown;
	node->_valueNode = value;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_hasModifiers = true;
	this->_modifiers = modifiers;
	return this;
}

void VarDeclarationNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "VarDecl: " + _varName);
	if (this->_hasModifiers)
	{
		file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
		this->_modifiers->generateDot(file);
	}
	if (this->_type == VarDeclType::TypeKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_typeNode->_id, "type");
		this->_typeNode->generateDot(file);
	}
	if (this->_type == VarDeclType::ValueKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_valueNode->_id, "value");
		this->_valueNode->generateDot(file);
	}
}

void VarDeclarationNode::fillTable(ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	switch (this->_type)
	{
	case TypeKnown:
		currentMethod->varTable->addLocalVar(this->_varName, this->_typeNode, currentClass->constants);
		break;
	case ValueAndTypeKnown:
		currentMethod->varTable->addLocalVar(this->_varName, this->_typeNode, currentClass->constants);
		break;
	default:
		throw std::runtime_error("Var declaration with enum type " + std::to_string(this->_type) + " is unsupported!");
		break;
	}
}

std::vector<char> VarDeclarationNode::generateCode(ClassTableElement * currentClass, MethodTableElement * currentMethod)
{
	std::vector<char> code = {};
	switch (this->_type)
	{
	case TypeKnown:
		//No code required for this type
		break;
	default:
		throw std::runtime_error("Var decl with enum type " + std::to_string(this->_type) + " is not supported!");
		break;
	}
	return code;
}

SemanticsBase* VarDeclarationNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	
	auto valueKnown = this->_type == ValueKnown || this->_type == ValueAndTypeKnown;
	auto typeKnown = this->_type == TypeKnown || this->_type == ValueAndTypeKnown;

	if (valueKnown)
	{
		this->_valueNode = this->_valueNode->semanticsTransform(stack)->typecast<ExprNode>();

		auto typeNode = this->_typeNode;
		if (!typeKnown)
		{
			typeNode = this->_valueNode->evaluateType();
		}

		auto thisNode = VarDeclarationNode::createFromType(this->_varName, typeNode);

		auto thisStmtList = stack.getClosest<StmtListNode>();
		auto thisStmt = stack.getClosest<StmtNode>();
		
		auto assignmentNode = StmtNode::createStmtAssignment(ExprNode::createId(this->_varName), this->_valueNode);

		thisStmtList->appendNodeAfterNode(assignmentNode, thisStmt);

		return thisNode;
	}

	return this;
}

std::string VarDeclarationListNode::getName()
{
	return "VarDeclList";
}

VarDeclarationListNode* VarDeclarationListNode::addModifiers(AccessModifierListNode* modifiers)
{
	for (auto& varDecl : this->_vec)
	{
		varDecl->addModifiers(modifiers);
	}
	return this;
}

void VarDeclarationListNode::fillTable( ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(currentClass, currentMethod);
	}
}

std::vector<char> VarDeclarationListNode::generateCode(ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	std::vector<char> code = {};
	for (auto& elem : _vec)
	{
		appendVecToVec(code, elem->generateCode(currentClass, currentMethod));
	}
	return code;
}

SemanticsBase* VarDeclarationListNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	for (auto& elem : _vec)
	{
		elem = elem->semanticsTransform(stack)->typecast<VarDeclarationNode>();
	}
	return this;
}
