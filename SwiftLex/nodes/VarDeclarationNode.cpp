#include "VarDeclarationNode.h"
#include "TypeNode.h"
#include "ExprNode.h"
#include "AccessModifierNode.h"
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
	// Class Field
	int constantValueIndex;
	if (currentMethod == nullptr)
	{
		if (_modifiers == nullptr)
		{
			throw std::runtime_error("Modifier missing for field: \"" + _varName + "\" for class \"" + currentClass->nameStr + "\"");
		}

		switch (this->_type)
		{
		case ValueAndTypeKnown:
			break;
		case TypeKnown:
			if (this->_typeNode->_type == IntT) {
				constantValueIndex = currentClass->constants->findOrAddConstant(Integer_C, "", 0);
				auto accessFlags = this->_modifiers->getMethodAccessFlags();
				bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();
				if (isStatic)
				{
					currentClass->constants->findOrAddConstant(Utf8_C, "ConstantValue");
					currentClass->addStaticField(this->_modifiers->getFieldAccessFlags(), this->_varName, this->_typeNode, constantValueIndex);
				} else{
					std::runtime_error("Non static field does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");
				}
			}
			else {
				std::runtime_error("Type " + std::to_string(this->_typeNode->_type) + "does not support for field \"" + this->_varName + "\" in class \"" + currentClass->nameStr + "\n");
			}
			break;
		case ValueKnown:
			break;
		default:
			break;
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
			currentMethod->varTable->addLocalVar(this->_varName, this->_typeNode, currentClass->constants);
			break;
		case ValueKnown:
			break;
		default:
			break;
		}
	}

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
