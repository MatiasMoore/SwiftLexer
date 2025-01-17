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

VarDeclarationNode* VarDeclarationNode::setAsFieldDecl(bool flag)
{
	this->_isFieldDecl = flag;
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

void VarDeclarationNode::fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	// Class Field
	int constantValueIndex;
	if (currentMethod == nullptr)
	{
		if (_modifiers == nullptr)
		{
			throw std::runtime_error("Modifier missing for field: \"" + _varName + "\" for class \"" + currentClass->nameStr + "\"");
		}

		if (this->_type == ValueAndTypeKnown)
		{
			// Check if field type is INT
			if (this->_typeNode->_type != IntT)
				throw std::runtime_error("Type " + std::to_string(this->_typeNode->_type) + "does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");

			// Check if assignable value is INT
			if (this->_valueNode->_type != Int)
				throw std::runtime_error("Value type does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");

			// Check if field is static
			auto accessFlags = this->_modifiers->getMethodAccessFlags();
			bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();
			if (!isStatic)
				throw std::runtime_error("Non static field does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");

			// Create name for attribute
			currentClass->constants->findOrAddConstant(Utf8_C, "ConstantValue");

			// Create field
			constantValueIndex = currentClass->constants->findOrAddConstant(Integer_C, "", this->_valueNode->_intValue);
			currentClass->addStaticField(this->_modifiers->getFieldAccessFlags(), this->_varName, this->_typeNode->toDescriptor(classTable, currentClass, currentMethod), constantValueIndex);
		}
		else if (this->_type == TypeKnown)
		{
			// Check if field type is INT
			if (this->_typeNode->_type != IntT)
				throw std::runtime_error("Type " + std::to_string(this->_typeNode->_type) + "does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");
			
			// Check if field is static
			auto accessFlags = this->_modifiers->getMethodAccessFlags();
			bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();
			if (!isStatic)
				throw std::runtime_error("Non static field does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");

			// Create name for attribute
			currentClass->constants->findOrAddConstant(Utf8_C, "ConstantValue");

			// Create field
			constantValueIndex = currentClass->constants->findOrAddConstant(Integer_C, "", 0); // DEFAULT INTEGER = 0
			currentClass->addStaticField(this->_modifiers->getFieldAccessFlags(), this->_varName, this->_typeNode->toDescriptor(classTable, currentClass, currentMethod), constantValueIndex);
		}
		else if (this->_type == ValueKnown) 
		{
			throw std::runtime_error("Declaration usupported for type " + std::to_string(this->_type) + " for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");
		}
		else
		{
			throw std::runtime_error("Declaration usupported for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");
		}
	}

	// Local Variable
	if (currentMethod != nullptr) 
	{
		switch (this->_type)
		{
		case ValueAndTypeKnown:
			break;
		case TypeKnown:
			currentMethod->varTable->addLocalVar(this->_varName, this->_typeNode->toDescriptor(classTable, currentClass, currentMethod), currentClass->constants);
			break;
		case ValueKnown:
			break;
		default:
			break;
		}
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

	// Add default modiffiers
	if (!this->_hasModifiers)
	{
		this->_modifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Internal));
		this->_hasModifiers = true;
	}

	// Only if it's not a field declaration
	if (!this->_isFieldDecl)
	{
		auto valueKnown = this->_type == ValueKnown || this->_type == ValueAndTypeKnown;
		auto typeKnown = this->_type == TypeKnown || this->_type == ValueAndTypeKnown;

		if (valueKnown)
		{
			this->_valueNode = this->_valueNode->semanticsTransform(stack)->typecast<ExprNode>();

			auto typeNode = this->_typeNode;
			if (!typeKnown)
			{
				typeNode = TypeNode::createDynamicType(this->_valueNode);
			}

			auto thisNode = VarDeclarationNode::createFromType(this->_varName, typeNode);
			if (this->_hasModifiers)
			{
				thisNode->addModifiers(this->_modifiers);
			}
			else {
				throw std::runtime_error("Missing modiffiers");
			}

			auto thisStmtList = stack.getClosest<StmtListNode>();
			if (thisStmtList == nullptr)
				throw std::runtime_error("Critical error! Unable to find stmt list for var decl!");

			auto thisStmt = stack.getClosest<StmtNode>();
			if (thisStmt == nullptr)
				throw std::runtime_error("Critical error! Unable to find stmt for var decl!");

			auto assignmentNode = StmtNode::createStmtAssignment(ExprNode::createId(this->_varName), this->_valueNode);

			thisStmtList->appendNodeAfterNode(assignmentNode, thisStmt);

			return thisNode;
		}
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

VarDeclarationListNode* VarDeclarationListNode::setAsFieldDecl(bool flag)
{
	for (auto& elem : _vec)
	{
		elem->setAsFieldDecl(flag);
	}
	return this;
}

bool VarDeclarationListNode::isFieldDecl()
{
	for (auto& elem : _vec)
	{
		if (elem->_isFieldDecl)
			return true;
	}

	return false;
}

void VarDeclarationListNode::fillTable(ClassTable* classTable, ClassTableElement* currentClass, MethodTableElement* currentMethod)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(classTable, currentClass, currentMethod);
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
